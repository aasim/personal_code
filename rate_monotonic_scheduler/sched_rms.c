/* 
 * RMS for the Xen hypervisor
 * 
 * by Aasim Malladi, Sachin Saner
 * 
 * This file contains code for implementing the 
 * Rate Monotonic Scheduler for the Xen Hypervisor
 * 
 */

#include <xen/config.h>
#include <xen/init.h>
#include <xen/lib.h>
#include <xen/sched.h>
#include <xen/domain.h>
#include <xen/delay.h>
#include <xen/event.h>
#include <xen/time.h>
#include <xen/perfc.h>
#include <xen/sched-if.h>
#include <xen/softirq.h>
#include <asm/atomic.h>
#include <xen/errno.h>


/*
 * Constants
 */
#define		RMS_TIME_SLICE			10
#define		HIGHEST_UTILIZATION		99
#define IDLETASK(cpu)  ((struct vcpu *)per_cpu(schedule_data, cpu).idle)

/*
 * Global variables
 */
struct vcpu * 	most_eligible_vcpu = NULL;
struct vcpu * 	vcpu_list_head = NULL;
static int 	utilization = 0, u;
static int	lowest_period = 0;
spinlock_t 	lock;

/* This holds a count of the vcpus currently initialized. */
static int	vcpus = 0;

/* Get the last VCPU in our list */
struct vcpu * 
get_last_vcpu_in_list(struct vcpu * vc)
{
	return vc->next_vcpu == NULL ? vc : get_last_vcpu_in_list(vc->next_vcpu);
}

/* Add a VCPU to our list */
static int
rms_vcpu_init(struct vcpu *vc)
{
	unsigned long flags;
	spin_lock_irqsave(&lock, flags);
	/* TODO: Get c and p from the user. Currently we dont
 	 * know how to do that, so, here we set default values
 	 * for the sake of testing this approach.
 	 */

	switch (vcpus)
	{
		case 0:
			vc->c = 2;
			vc->p = 5;
			most_eligible_vcpu = vc;
			break;
		case 1:
			vc->c = 3;
			vc->p = 6;
			break;
		case 2:			
			vc->c = 1;
			vc->p = 12;
			break;
		case 3:
			vc->c = 1;
			vc->p = 14;
			break;

		case 4:
			vc->c = 1;
			vc->p = 16;
			break;
		case 5:
			vc->c = 1;
			vc->p = 18;
			break;
		case 6:
			vc->c = 1;
			vc->p = 20;
			break;
		case 7:
			vc->c = 1;
			vc->p = 22;
			break;
		case 8:
			vc->c = 2;
			vc->p = 25;
			break;
		case 9:
			vc->c = 2;
			vc->p = 27;
			break;
		default:
			/* Wont admit more than 10 CPUs :)
 			 */
			return -1;
	}

	vcpus ++;

	u = (int)vc->c;
	u *= 100; 
	u /= vc->p;
	
	if(u > HIGHEST_UTILIZATION)
		/* Cannot admit */
		return -1;
	else
		utilization += u;

	/* Add this VCPU to our list of VCPUs
  	 */
	if(vcpu_list_head == NULL)
	{
		/* This is the first VCPU */
		vcpu_list_head = vc;
		vc->next_vcpu = NULL;
		vc->prev_vcpu = NULL;
		printk("\nVCPU %d added to head of list\n", vc->vcpu_id);
	}
	else
	{
		/* Attach this VCPU at the end of our list
  		 */
		struct vcpu * temp = get_last_vcpu_in_list(vcpu_list_head);
		temp->next_vcpu = vc;
		vc->next_vcpu = NULL;
		vc->prev_vcpu = temp;
		printk("\nVCPU %d added to tail of list\n", vc->vcpu_id);
	}
	
	vc->time_slice = vc->c;
	vc->curr_p = vc->p;
	vc->sched_priv = NULL;
	spin_unlock_irqrestore(&lock, flags);
	/* All operations successful */
	return 0;
}

/* Remove a VCPU from our list */
static void
rms_vcpu_destroy(struct vcpu *vc)
{

	unsigned long flags;
	spin_lock_irqsave(&lock, flags);
	
	/* Deduct the utilization */
	utilization -= ((vc->c * 100) / vc->p );
	
	/* Remove the VCPU from our list */
	if(vc->prev_vcpu == NULL)
	{
		if(vc->next_vcpu == NULL)
			/* This is the first VCPU 
  			 * and no other VCPUs exist
  			 */
			vcpu_list_head = NULL;
		else
			/* Set the head as the next VCPU
  			 */
			vcpu_list_head = vc->next_vcpu;
			
	}
	else
	{
		if(vc->next_vcpu == NULL)
			/* This is the last VCPU in the list
 			 */
			vc->prev_vcpu->next_vcpu = NULL;
		else
		{
			vc->prev_vcpu->next_vcpu = vc->next_vcpu;
			vc->next_vcpu->prev_vcpu = vc->prev_vcpu;
		}	
	}
	
	printk("\nVCPU %d deleted from list", vc->vcpu_id);

	spin_unlock_irqrestore(&lock, flags);
}


void
update_times(struct vcpu * vc)
{
	if(vc == NULL)
		/* This shouldnt happen */
		return;
	else
	{
		/* If a VCPU still has some time_slice remaining,
  		 * see if its deadline is the lowest, and make it the
  		 * most eligible VCPU
  		 * 
  		 * Ignore VCPUs with no time_slice left.
  		 */
		if(vc->time_slice > 0 && vcpu_runnable(vc))
		{
			if(vc->p < lowest_period)
			{
				/* This is the VCPU with the least
 				 * period, so, this is the most eligible of
  				 * them all!
  				 */
				most_eligible_vcpu = vc;
				lowest_period = vc->p;
				printk("\nVCPU %d is eligible to run with time_slice %d", vc->vcpu_id, vc->time_slice);
			}	
		}
		
		vc->curr_p--;
		
		if(vc->curr_p <= 0)
		{
			/* Refill */
			vc->time_slice = vc->c;
			vc->curr_p = vc->p;
			printk("\nRefill time_slice for VCPU %d", vc->vcpu_id);
		}
		
	}
	
	
	/* Call recursively for all the remaining VCPUs in the list/
  	 */
	if(vc->next_vcpu != NULL)
		update_times(vc->next_vcpu);
	
}

static struct task_slice
rms_do_schedule(s_time_t now)
{
	struct task_slice ret;

	ret.time = MILLISECS(RMS_TIME_SLICE);
	
//	most_eligible_vcpu = NULL;
//	lowest_period = 99999;
	
	/* Update the runtimes and wait-times of each VCPU
  	 *
  	 * TODO: Make the runqueue sorted by lowest period,
  	 * so, each time we dont have to update the entire 
  	 * queue. Here we will pick only the first element
  	 * from the queue and run it.
  	 */
	printk("\nIn update times .. ");
	
//	update_times(vcpu_list_head);
//	printk("Returned from update times\n");

	if(vcpu_list_head != NULL && vcpu_runnable(vcpu_list_head) && vcpu_list_head->time_slice > 0)
	{
		vcpu_list_head->time_slice--;
		ret.task = vcpu_list_head;
	}
	else if(vcpu_list_head->next_vcpu != NULL && vcpu_runnable(vcpu_list_head->next_vcpu) && (vcpu_list_head->next_vcpu)->time_slice > 0)
	{
		(vcpu_list_head->next_vcpu)->time_slice--;
		ret.task = vcpu_list_head->next_vcpu;
	}
	else
	{
		int cpu = smp_processor_id();
		ret.task = IDLETASK(cpu);
	}

	vcpu_list_head->curr_p--;
	if(vcpu_list_head->curr_p <= 0)
	{
		vcpu_list_head->curr_p = vcpu_list_head->p;
		vcpu_list_head->time_slice = vcpu_list_head->c;
	}

	(vcpu_list_head->next_vcpu)->curr_p--;
	if((vcpu_list_head->next_vcpu)->curr_p <= 0)
	{
		(vcpu_list_head->next_vcpu)->curr_p = (vcpu_list_head->next_vcpu)->p;
		(vcpu_list_head->next_vcpu)->time_slice = (vcpu_list_head->next_vcpu)->c;
	}

	/* Return this structure */
	return ret;
}


/*
 * Initialization of rqs etc.
 */
static void
rms_init(void)
{
	spin_lock_init(&lock);
	vcpu_list_head = NULL;
	most_eligible_vcpu = NULL;
	utilization = 0;
	lowest_period = 0;
}

/* The struct for our rms */
struct scheduler sched_rms = {
    .name           = "RMS",
    .opt_name       = "rms",
    .sched_id       = XEN_SCHEDULER_SEDF,

    .init_vcpu      = rms_vcpu_init,
    .destroy_vcpu   = rms_vcpu_destroy,

    .do_schedule    = rms_do_schedule,


    .init           = rms_init,
};

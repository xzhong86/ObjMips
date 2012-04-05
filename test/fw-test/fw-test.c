#include <stdlib.h>
#include <test/fw_test.h>
#include <init.h>
#include <test/test-head.h>

struct dev * head=NULL;

int fwdev_register(struct fw_dev *fwdev)
{
	struct dev * dev;
	dev = (struct dev *)malloc (sizeof(struct dev));
	dev->fwdev = fwdev;
	dev->busy = 1;
	dev->state = ST_INIT;
	//insert to list
	dev->next = head;
	head = dev;
       	return 0;
}

int fw_test(void)
{
	struct dev *dev;

	do{
		for(dev = head; dev != NULL; dev = dev->next){
		    	if(dev->busy == 0)
				dev->busy = 1;
			switch(dev->state){
			case ST_INIT:
				dev->fwdev->prepare(dev->fwdev);
				if(dev->fwdev->reset != NULL)
					dev->fwdev->reset(dev->fwdev);
				dev->state = ST_READY;
			case ST_READY:
				dev->fwdev->start(dev->fwdev);
				dev->state = ST_RUNNING;
			case ST_RUNNING:
				if(dev->fwdev->ask != NULL)
					dev->fwdev->ask(dev->fwdev);
			case ST_FINISHED:
				if(dev->fwdev->finish != NULL)
					dev->fwdev->finish(dev->fwdev);
			
				if(dev->fwdev->check(dev->fwdev) == 0){
					if(dev->fwdev->reset != NULL)
						dev->fwdev->reset(dev->fwdev);
					dev->state = ST_READY;
				}
				else{
					if(dev->fwdev->stop != NULL)
						dev->fwdev->stop(dev->fwdev);
					dev->state = ST_STOPPED;
					break;
				}
			case ST_ERROR:
				if(dev->fwdev->stop != NULL)								
					dev->fwdev->stop(dev->fwdev);
				dev->state = ST_STOPPED;
				break;
			default:printk("the dev is free\n");
				break;
				}
			dev->busy = 0;
			}
      	}while(1);
       	return 0;
}

int fw_finish(struct fw_dev *fwdev)
{
	struct dev *dev;
	for(dev = head; dev != NULL; dev = dev->next){
		if(dev->fwdev == fwdev)
			dev->state = ST_FINISHED;
	}
	return 0;
}
int fw_error(struct fw_dev *fwdev)
{
	struct dev *dev;
	for(dev = head; dev != NULL; dev = dev->next){
		if(dev->fwdev == fwdev)
			dev->state = ST_ERROR;
	}
	return 0;
}

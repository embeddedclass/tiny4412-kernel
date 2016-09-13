#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/poll.h>
#include <linux/sched.h>
#include <linux/irq.h>
#include <asm/irq.h>
#include <asm/io.h>
#include <linux/interrupt.h>
#include <asm/uaccess.h>
#include <mach/hardware.h>
#include <linux/platform_device.h>
#include <linux/cdev.h>
#include <linux/miscdevice.h>
#include <linux/gpio.h>

#include <mach/map.h>
#include <mach/gpio.h>
#include <mach/regs-clock.h>
#include <mach/regs-gpio.h>

irqreturn_t my_button_isr(int irq, void *dev_instance)
{
	printk("Call %s\n",__func__);
        return IRQ_NONE;
}

static int my_button_probe(struct platform_device *pdev)
{
	printk("Call [%s]\n",__func__);

	int ret, button_irq;
	struct resource *but_res;

	but_res = platform_get_resource(pdev, IORESOURCE_IRQ, 0);
	if (!but_res)
		printk("faild to get button resource!!!\n");

	button_irq = gpio_to_irq(but_res->start);
        ret = request_irq(button_irq, my_button_isr, IRQ_TYPE_EDGE_FALLING, "my_button", NULL);
	if (ret)
		printk("request_irq() failed (%d)\n", ret);
	
	return ret;
}

static int my_button_remove(struct platform_device *dev)
{
	printk("Call %s\n",__func__);
	return 0;
}

static struct platform_driver my_button_driver = {
	.driver = {
		.name		= "my_button_platform",
		.owner		= THIS_MODULE,
	},
	.probe		= my_button_probe,
	.remove		= my_button_remove,
};

static int __init my_button_init(void)
{
	return platform_driver_register(&my_button_driver);
}

static void __exit my_button_exit(void)
{
	platform_driver_unregister(&my_button_driver);
}

module_init(my_button_init);
module_exit(my_button_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Terence");

/* SPDX-License-Identifier: GPL-2.0 or MIT                               */
/* Copyright(c) 2021 Ross K.Snider. All rights reserved.                 */
/*-------------------------------------------------------------------------
 * Description:  Linux Platform Device Driver for the 
 *               hps_color_led component
 * ------------------------------------------------------------------------
 * Authors : Ross K. Snider and Trevor Vannoy
 * Company : Montana State University
 * Create Date : November 10, 2021
 * Revision : 1.0
 * License : GPL-2.0 or MIT (opensource.org / licenses / MIT, GPL-2.0)
-------------------------------------------------------------------------*/
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/mod_devicetable.h>
#include <linux/types.h>
#include <linux/io.h>
#include <linux/mutex.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/uaccess.h>
/*#include "fp_conversions.h"*/

/*-----------------------------------------------------------------------*/
/* DEFINE STATEMENTS                                                     */
/*-----------------------------------------------------------------------*/
/* Define the Component Register Offsets*/
#define REG0_red_OFFSET 0x00
#define REG1_green_OFFSET 0x04
#define REG2_blue_OFFSET 0x08
#define REG3_SW_OFFSET 0x0C

/* Memory span of all registers (used or not) in the                     */
/* component hps_color_led                                            */
#define SPAN 0x20


/*-----------------------------------------------------------------------*/
/* hps_color_led device structure                                     */
/*-----------------------------------------------------------------------*/
/*
 * struct  hps_color_led_dev - Private hps_color_led device struct.
 * @miscdev: miscdevice used to create a char device 
 *           for the hps_color_led component
 * @base_addr: Base address of the hps_color_led component
 * @lock: mutex used to prevent concurrent writes 
 *        to the hps_color_led component
 *
 * An hps_color_led_dev struct gets created for each hps_color_led 
 * component in the system.
 */
struct hps_color_led_dev {
	struct miscdevice miscdev;
	void __iomem *base_addr;
	struct mutex lock;
};

/*-----------------------------------------------------------------------*/
/* REG0: HPS_LED_control register read function show()                   */
/*-----------------------------------------------------------------------*/
/*
 * hps_led_control_show() - Return the hps_led_control value 
 *                          to user-space via sysfs.
 * @dev: Device structure for the hps_color_led component. This 
 *       device struct is embedded in the hps_color_led' device struct.
 * @attr: Unused.
 * @buf: Buffer that gets returned to user-space.
 *
 * Return: The number of bytes read.
 */



/*-----------------------------------------------------------------------*/
/* TODO: Add show() and store() functions for                            */
/* Registers REG1 (red) and REG2 (blue_sec)                    */
/* in component hps_color_led                                         */
/*-----------------------------------------------------------------------*/
/* Add here...  */
/*
-----------------------
REG1 red
-----------------------
*/
static ssize_t red_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	u32 red;
	// Get the private hps_color_led data out of the dev struct
	struct hps_color_led_dev *priv = dev_get_drvdata(dev);

	red = ioread32(priv->base_addr + REG0_red_OFFSET);

	return scnprintf(buf, PAGE_SIZE, "%u\n", red);
}

static ssize_t red_store(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t size)
{
	u32 red;
	int ret;
	struct hps_color_led_dev *priv = dev_get_drvdata(dev);

	// Parse the string we received as a u8
	// See https://elixir.bootlin.com/linux/latest/source/lib/kstrtox.c#L289 
	ret = kstrtou32(buf, 0, &red);
	if (ret < 0) {
		// kstrtou16 returned an error
		return ret;
	}

	iowrite32(red, priv->base_addr + REG0_red_OFFSET);

	// Write was succesful, so we return the number of bytes we wrote.
	return size;
}
/*
-----------------------
REG2 green_Sec
-----------------------
*/
static ssize_t green_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	u32 green;
	// Get the private hps_color_led data out of the dev struct
	struct hps_color_led_dev *priv = dev_get_drvdata(dev);

	green = ioread32(priv->base_addr + REG1_green_OFFSET);

	return scnprintf(buf, PAGE_SIZE, "%u\n", green);
}

static ssize_t green_store(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t size)
{
	u32 green;
	int ret;
	struct hps_color_led_dev *priv = dev_get_drvdata(dev);

	// Parse the string we received as a u8
	// See https://elixir.bootlin.com/linux/latest/source/lib/kstrtox.c#L289 
	ret = kstrtou32(buf, 0, &green);
	if (ret < 0) {
		// kstrtou16 returned an error
		return ret;
	}

	iowrite32(green, priv->base_addr + REG1_green_OFFSET);

	// Write was succesful, so we return the number of bytes we wrote.
	return size;
}

/*-----------------------------------------------------------------------*/
/* REG3: blue register read function show()                           */
/*-----------------------------------------------------------------------*/
/*
 * green_show() - Return the green value to user-space via sysfs.
 * @dev: Device structure for the hps_color_led component. This 
 *       device struct is embedded in the hps_color_led' platform 
 *       device struct.
 * @attr: Unused.
 * @buf: Buffer that gets returned to user-space.
 *
 * Return: The number of bytes read.
 */
static ssize_t blue_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	u32 blue;
	struct hps_color_led_dev *priv = dev_get_drvdata(dev);

	blue = ioread32(priv->base_addr + REG2_blue_OFFSET);

	return scnprintf(buf, PAGE_SIZE, "%u\n", blue);
}
/*-----------------------------------------------------------------------*/
/* REG3: blue register write function store()                         */
/*-----------------------------------------------------------------------*/
/*
 * blue_store() - Store the blue value.
 * @dev: Device structure for the hps_color_led component. This 
 *       device struct is embedded in the hps_color_led' platform 
 *       device struct.
 * @attr: Unused.
 * @buf: Buffer that contains the  hps_color_led value being written.
 * @size: The number of bytes being written.
 *
 * Return: The number of bytes stored.
 */
static ssize_t blue_store(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t size)
{
	u32 blue;
	int ret;
	struct hps_color_led_dev *priv = dev_get_drvdata(dev);

	// Parse the string we received as a u8
	// See https://elixir.bootlin.com/linux/latest/source/lib/kstrtox.c#L289 
	ret = kstrtou32(buf, 0, &blue);
	if (ret < 0) {
		// kstrtou16 returned an error
		return ret;
	}

	iowrite32(blue, priv->base_addr + REG2_blue_OFFSET);

	// Write was succesful, so we return the number of bytes we wrote.
	return size;
}


/*-----------------------------------------------------------------------*/
/* sysfs Attributes                                                      */
/*-----------------------------------------------------------------------*/
// Define sysfs attributes

static DEVICE_ATTR_RW(red);
static DEVICE_ATTR_RW(green);
/* TODO: Add the attributes for REG1 and REG2 using register names       */
static DEVICE_ATTR_RW(blue);            // Attribute for REG3

// Create an atribute group so the device core can 
// export the attributes for us.
static struct attribute *hps_color_led_attrs[] = {
	&dev_attr_red.attr,
	&dev_attr_green.attr,
/* TODO: Add the attribute entries for REG1 and REG2 using register names*/
	&dev_attr_blue.attr,
	NULL,
};
ATTRIBUTE_GROUPS(hps_color_led);


/*-----------------------------------------------------------------------*/
/* File Operations read()                                                */
/*-----------------------------------------------------------------------*/
/*
 * hps_color_led_read() - Read method for the hps_color_led char device
 * @file: Pointer to the char device file struct.
 * @buf: User-space buffer to read the value into.
 * @count: The number of bytes being requested.
 * @offset: The byte offset in the file being read from.
 *
 * Return: On success, the number of bytes written is returned and the
 * offset @offset is advanced by this number. On error, a negative error
 * value is returned.
 */
static ssize_t hps_color_led_read(struct file *file, char __user *buf,
	size_t count, loff_t *offset)
{
	size_t ret;
	u32 val;

	loff_t pos = *offset;

	/*
	 * Get the device's private data from the file struct's private_data
	 * field. The private_data field is equal to the miscdev field in the
	 * hps_color_led_dev struct. container_of returns the 
     * hps_color_led_dev struct that contains the miscdev in private_data.
	 */
	struct hps_color_led_dev *priv = container_of(file->private_data,
	                            struct hps_color_led_dev, miscdev);

	// Check file offset to make sure we are reading to a valid location.
	if (pos < 0) {
		// We can't read from a negative file position.
		return -EINVAL;
	}
	if (pos >= SPAN) {
		// We can't read from a position past the end of our device.
		return 0;
	}
	if ((pos % 0x4) != 0) {
		/*
		 * Prevent unaligned access. Even though the hardware
		 * technically supports unaligned access, we want to
		 * ensure that we only access 32-bit-aligned addresses
		 * because our registers are 32-bit-aligned.
		 */
		pr_warn("hps_color_led_read: unaligned access\n");
		return -EFAULT;
	}

	// If the user didn't request any bytes, don't return any bytes :)
	if (count == 0) {
		return 0;
	}

	// Read the value at offset pos.
	val = ioread32(priv->base_addr + pos);

	ret = copy_to_user(buf, &val, sizeof(val));
	if (ret == sizeof(val)) {
		// Nothing was copied to the user.
		pr_warn("hps_color_led_read: nothing copied\n");
		return -EFAULT;
	}

	// Increment the file offset by the number of bytes we read.
	*offset = pos + sizeof(val);

	return sizeof(val);
}
/*-----------------------------------------------------------------------*/
/* File Operations write()                                               */
/*-----------------------------------------------------------------------*/
/*
 * hps_color_led_write() - Write method for the hps_color_led char device
 * @file: Pointer to the char device file struct.
 * @buf: User-space buffer to read the value from.
 * @count: The number of bytes being written.
 * @offset: The byte offset in the file being written to.
 *
 * Return: On success, the number of bytes written is returned and the
 * offset @offset is advanced by this number. On error, a negative error
 * value is returned.
 */
static ssize_t hps_color_led_write(struct file *file, const char __user *buf,
	size_t count, loff_t *offset)
{
	size_t ret;
	u32 val;

	loff_t pos = *offset;

	/*
	 * Get the device's private data from the file struct's private_data
	 * field. The private_data field is equal to the miscdev field in the
	 * hps_color_led_dev struct. container_of returns the 
     * hps_color_led_dev struct that contains the miscdev in private_data.
	 */
	struct hps_color_led_dev *priv = container_of(file->private_data,
	                              struct hps_color_led_dev, miscdev);

	// Check file offset to make sure we are writing to a valid location.
	if (pos < 0) {
		// We can't write to a negative file position.
		return -EINVAL;
	}
	if (pos >= SPAN) {
		// We can't write to a position past the end of our device.
		return 0;
	}
	if ((pos % 0x4) != 0) {
		/*
		 * Prevent unaligned access. Even though the hardware
		 * technically supports unaligned access, we want to
		 * ensure that we only access 32-bit-aligned addresses
		 * because our registers are 32-bit-aligned.
		 */
		pr_warn("hps_color_led_write: unaligned access\n");
		return -EFAULT;
	}

	// If the user didn't request to write anything, return 0.
	if (count == 0) {
		return 0;
	}

	mutex_lock(&priv->lock);

	ret = copy_from_user(&val, buf, sizeof(val));
	if (ret == sizeof(val)) {
		// Nothing was copied from the user.
		pr_warn("hps_color_led_write: nothing copied from user space\n");
		ret = -EFAULT;
		goto unlock;
	}

	// Write the value we were given at the address offset given by pos.
	iowrite32(val, priv->base_addr + pos);

	// Increment the file offset by the number of bytes we wrote.
	*offset = pos + sizeof(val);

	// Return the number of bytes we wrote.
	ret = sizeof(val);

unlock:
	mutex_unlock(&priv->lock);
	return ret;
}


/*-----------------------------------------------------------------------*/
/* File Operations Supported                                             */
/*-----------------------------------------------------------------------*/
/*
 *  hps_color_led_fops - File operations supported by the  
 *                          hps_color_led driver
 * @owner: The hps_color_led driver owns the file operations; this 
 *         ensures that the driver can't be removed while the 
 *         character device is still in use.
 * @read: The read function.
 * @write: The write function.
 * @llseek: We use the kernel's default_llseek() function; this allows 
 *          users to change what position they are writing/reading to/from.
 */
static const struct file_operations  hps_color_led_fops = {
	.owner = THIS_MODULE,
	.read = hps_color_led_read,
	.write = hps_color_led_write,
	.llseek = default_llseek,
};


/*-----------------------------------------------------------------------*/
/* Platform Driver Probe (Initialization) Function                       */
/*-----------------------------------------------------------------------*/
/*
 * hps_color_led_probe() - Initialize device when a match is found
 * @pdev: Platform device structure associated with our 
 *        hps_color_led device; pdev is automatically created by the 
 *        driver core based upon our hps_color_led device tree node.
 *
 * When a device that is compatible with this hps_color_led driver 
 * is found, the driver's probe function is called. This probe function 
 * gets called by the kernel when an hps_color_led device is found 
 * in the device tree.
 */
static int hps_color_led_probe(struct platform_device *pdev)
{
	struct hps_color_led_dev *priv;
	int ret;

	/*
	 * Allocate kernel memory for the hps_color_led device and set it to 0.
	 * GFP_KERNEL specifies that we are allocating normal kernel RAM;
	 * see the kmalloc documentation for more info. The allocated memory
	 * is automatically freed when the device is removed.
	 */
	priv = devm_kzalloc(&pdev->dev, sizeof(struct hps_color_led_dev), GFP_KERNEL);
	if (!priv) {
		pr_err("Failed to allocate kernel memory for hps_led_pattern\n");
		return -ENOMEM;
	}

	/*
	 * Request and remap the device's memory region. Requesting the region
	 * make sure nobody else can use that memory. The memory is remapped
	 * into the kernel's virtual address space becuase we don't have access
	 * to physical memory locations.
	 */
	priv->base_addr = devm_platform_ioremap_resource(pdev, 0);
	if (IS_ERR(priv->base_addr)) {
		pr_err("Failed to request/remap platform device resource (hps_color_led)\n");
		return PTR_ERR(priv->base_addr);
	}

	// Initialize the misc device parameters
	priv->miscdev.minor = MISC_DYNAMIC_MINOR;
	priv->miscdev.name = "hps_color_led";
	priv->miscdev.fops = &hps_color_led_fops;
	priv->miscdev.parent = &pdev->dev;
	priv->miscdev.groups = hps_color_led_groups;

	// Register the misc device; this creates a char dev at 
    // /dev/hps_color_led
	ret = misc_register(&priv->miscdev);
	if (ret) {
		pr_err("Failed to register misc device for hps_color_led\n");
		return ret;
	}

	// Attach the hps_color_led' private data to the 
    // platform device's struct.
	platform_set_drvdata(pdev, priv);

	pr_info("hps_color_led_probe successful\n");

	return 0;
}

/*-----------------------------------------------------------------------*/
/* Platform Driver Remove Function                                       */
/*-----------------------------------------------------------------------*/
/*
 * hps_color_led_remove() - Remove an hps_color_led device.
 * @pdev: Platform device structure associated with our hps_color_led device.
 *
 * This function is called when an hps_color_led devicee is removed or
 * the driver is removed.
 */
static int hps_color_led_remove(struct platform_device *pdev)
{
	// Get thehps_color_led' private data from the platform device.
	struct hps_color_led_dev *priv = platform_get_drvdata(pdev);

	// Deregister the misc device and remove the /dev/hps_color_led file.
	misc_deregister(&priv->miscdev);

	pr_info("hps_color_led_remove successful\n");

	return 0;
}

/*-----------------------------------------------------------------------*/
/* Compatible Match String                                               */
/*-----------------------------------------------------------------------*/
/*
 * Define the compatible property used for matching devices to this driver,
 * then add our device id structure to the kernel's device table. For a 
 * device to be matched with this driver, its device tree node must use the 
 * same compatible string as defined here.
 */
static const struct of_device_id hps_color_led_of_match[] = {
    // ****Note:**** This .compatible string must be identical to the 
    // .compatible string in the Device Tree Node for hps_color_led
	{ .compatible = "ajm,HPS_Color_LED", },  
	{ }
};
MODULE_DEVICE_TABLE(of, hps_color_led_of_match);

/*-----------------------------------------------------------------------*/
/* Platform Driver Structure                                             */
/*-----------------------------------------------------------------------*/
/*
 * struct hps_color_led_driver - Platform driver struct for the 
 *                                  hps_color_led driver
 * @probe: Function that's called when a device is found
 * @remove: Function that's called when a device is removed
 * @driver.owner: Which module owns this driver
 * @driver.name: Name of the hps_color_led driver
 * @driver.of_match_table: Device tree match table
 * @driver.dev_groups: hps_color_led sysfs attribute group; this
 *                     allows the driver core to create the
 *                     attribute(s) without race conditions.
 */
static struct platform_driver hps_color_led_driver = {
	.probe = hps_color_led_probe,
	.remove = hps_color_led_remove,
	.driver = {
		.owner = THIS_MODULE,
		.name = "hps_color_led",
		.of_match_table = hps_color_led_of_match,
		.dev_groups = hps_color_led_groups,
	},
};

/*
 * We don't need to do anything special in module init/exit.
 * This macro automatically handles module init/exit.
 */
module_platform_driver(hps_color_led_driver);

MODULE_LICENSE("Dual MIT/GPL");
MODULE_AUTHOR("Anthony Meader");  // Adapted from Trevor Vannoy's Echo Driver
MODULE_DESCRIPTION("hps_color_led driver");
MODULE_VERSION("1.0");


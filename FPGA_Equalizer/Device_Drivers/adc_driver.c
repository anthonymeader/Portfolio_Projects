/* SPDX-License-Identifier: GPL-2.0 or MIT                               */
/* Copyright(c) 2021 Ross K.Snider. All rights reserved.                 */
/*-------------------------------------------------------------------------
 * Description:  Linux Platform Device Driver for the 
 *               adc_color_cd component
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
#define REG0_CH0_OFFSET 0x00
#define REG1_CH1_OFFSET 0x04
#define REG2_CH2_OFFSET 0x08
#define REG3_CH3_OFFSET 0x0C
#define REG4_CH4_OFFSET 0x10
#define REG5_CH5_OFFSET 0x14
#define REG6_CH6_OFFSET 0x18
#define REG7_CH7_OFFSET 0x1C
/* Memory span of all registers (used or not) in the                     */
/* component adc                                            */
#define SPAN 0x20


/*-----------------------------------------------------------------------*/
/* adc device structure                                     */
/*-----------------------------------------------------------------------*/
/*
 * struct  adc_dev - Private adc device struct.
 * @miscdev: miscdevice used to create a char device 
 *           for the adc component
 * @base_addr: Base address of the adc component
 * @lock: mutex used to prevent concurrent writes 
 *        to the adc component
 *
 * An adc_dev struct gets created for each adc 
 * component in the system.
 */
struct adc_dev {
	struct miscdevice miscdev;
	void __iomem *base_addr;
	struct mutex lock;
};

/*-----------------------------------------------------------------------*/
/* REG0: adc_LED_control register read function show()                   */
/*-----------------------------------------------------------------------*/
/*
 * adc_led_control_show() - Return the adc_led_control value 
 *                          to user-space via sysfs.
 * @dev: Device structure for the adc component. This 
 *       device struct is embedded in the adc' device struct.
 * @attr: Unused.
 * @buf: Buffer that gets returned to user-space.
 *
 * Return: The number of bytes read.
 */



/*-----------------------------------------------------------------------*/
/* TODO: Add show() and store() functions for                            */
/* Registers REG1 (ch2) and REG2 (ch3_sec)                    */
/* in component adc                                         */
/*-----------------------------------------------------------------------*/
/* Add here...  */
static ssize_t ch0_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	u32 ch0;
	// Get the private adc data out of the dev struct
	struct adc_dev *priv = dev_get_drvdata(dev);

	ch0 = ioread32(priv->base_addr + REG0_CH0_OFFSET);

	return scnprintf(buf, PAGE_SIZE, "%u\n", ch0);
}

static ssize_t ch0_store(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t size)
{
	u32 ch0;
	int ret;
	struct adc_dev *priv = dev_get_drvdata(dev);

	// Parse the string we received as a u8
	// See https://elixir.bootlin.com/linux/latest/source/lib/kstrtox.c#L289 
	ret = kstrtou32(buf, 0, &ch0);
	if (ret < 0) {
		// kstrtou16 returned an error
		return ret;
	}

	iowrite32(ch0, priv->base_addr + REG0_CH0_OFFSET);

	// Write was succesful, so we return the number of bytes we wrote.
	return size;
}

static ssize_t ch1_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	u32 ch1;
	// Get the private adc data out of the dev struct
	struct adc_dev *priv = dev_get_drvdata(dev);

	ch1 = ioread32(priv->base_addr + REG1_CH1_OFFSET);

	return scnprintf(buf, PAGE_SIZE, "%u\n", ch1);
}

static ssize_t ch1_store(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t size)
{
	u32 ch1;
	int ret;
	struct adc_dev *priv = dev_get_drvdata(dev);

	// Parse the string we received as a u8
	// See https://elixir.bootlin.com/linux/latest/source/lib/kstrtox.c#L289 
	ret = kstrtou32(buf, 0, &ch1);
	if (ret < 0) {
		// kstrtou16 returned an error
		return ret;
	}

	iowrite32(ch1, priv->base_addr + REG1_CH1_OFFSET);

	// Write was succesful, so we return the number of bytes we wrote.
	return size;
}

/*
-----------------------
REG1 ch2
-----------------------
*/
static ssize_t ch2_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	u32 ch2;
	// Get the private adc data out of the dev struct
	struct adc_dev *priv = dev_get_drvdata(dev);

	ch2 = ioread32(priv->base_addr + REG2_CH2_OFFSET);

	return scnprintf(buf, PAGE_SIZE, "%u\n", ch2);
}

static ssize_t ch2_store(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t size)
{
	u32 ch2;
	int ret;
	struct adc_dev *priv = dev_get_drvdata(dev);

	// Parse the string we received as a u8
	// See https://elixir.bootlin.com/linux/latest/source/lib/kstrtox.c#L289 
	ret = kstrtou32(buf, 0, &ch2);
	if (ret < 0) {
		// kstrtou16 returned an error
		return ret;
	}

	iowrite32(ch2, priv->base_addr + REG2_CH2_OFFSET);

	// Write was succesful, so we return the number of bytes we wrote.
	return size;
}
/*
-----------------------
REG2 ch3_Sec
-----------------------
*/
static ssize_t ch3_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	u32 ch3;
	// Get the private adc data out of the dev struct
	struct adc_dev *priv = dev_get_drvdata(dev);

	ch3 = ioread32(priv->base_addr + REG3_CH3_OFFSET);

	return scnprintf(buf, PAGE_SIZE, "%u\n", ch3);
}

static ssize_t ch3_store(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t size)
{
	u32 ch3;
	int ret;
	struct adc_dev *priv = dev_get_drvdata(dev);

	// Parse the string we received as a u8
	// See https://elixir.bootlin.com/linux/latest/source/lib/kstrtox.c#L289 
	ret = kstrtou32(buf, 0, &ch3);
	if (ret < 0) {
		// kstrtou16 returned an error
		return ret;
	}

	iowrite32(ch3, priv->base_addr + REG3_CH3_OFFSET);

	// Write was succesful, so we return the number of bytes we wrote.
	return size;
}

/*-----------------------------------------------------------------------*/
/* REG3: ch4 register read function show()                           */
/*-----------------------------------------------------------------------*/
/*
 * ch4_show() - Return the ch4 value to user-space via sysfs.
 * @dev: Device structure for the adc component. This 
 *       device struct is embedded in the adc' platform 
 *       device struct.
 * @attr: Unused.
 * @buf: Buffer that gets returned to user-space.
 *
 * Return: The number of bytes read.
 */
static ssize_t ch4_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	u32 ch4;
	struct adc_dev *priv = dev_get_drvdata(dev);

	ch4 = ioread32(priv->base_addr + REG4_CH4_OFFSET);

	return scnprintf(buf, PAGE_SIZE, "%u\n", ch4);
}
/*-----------------------------------------------------------------------*/
/* REG3: ch4 register write function store()                         */
/*-----------------------------------------------------------------------*/
/*
 * ch4_store() - Store the ch4 value.
 * @dev: Device structure for the adc component. This 
 *       device struct is embedded in the adc' platform 
 *       device struct.
 * @attr: Unused.
 * @buf: Buffer that contains the  adc value being written.
 * @size: The number of bytes being written.
 *
 * Return: The number of bytes stored.
 */
static ssize_t ch4_store(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t size)
{
	u32 ch4;
	int ret;
	struct adc_dev *priv = dev_get_drvdata(dev);

	// Parse the string we received as a u8
	// See https://elixir.bootlin.com/linux/latest/source/lib/kstrtox.c#L289 
	ret = kstrtou32(buf, 0, &ch4);
	if (ret < 0) {
		// kstrtou16 returned an error
		return ret;
	}

	iowrite32(ch4, priv->base_addr + REG4_CH4_OFFSET);

	// Write was succesful, so we return the number of bytes we wrote.
	return size;
}
static ssize_t ch5_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	u32 ch5;
	// Get the private adc data out of the dev struct
	struct adc_dev *priv = dev_get_drvdata(dev);

	ch5 = ioread32(priv->base_addr + REG5_CH5_OFFSET);

	return scnprintf(buf, PAGE_SIZE, "%u\n", ch5);
}

static ssize_t ch5_store(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t size)
{
	u32 ch5;
	int ret;
	struct adc_dev *priv = dev_get_drvdata(dev);

	// Parse the string we received as a u8
	// See https://elixir.bootlin.com/linux/latest/source/lib/kstrtox.c#L289 
	ret = kstrtou32(buf, 0, &ch5);
	if (ret < 0) {
		// kstrtou16 returned an error
		return ret;
	}

	iowrite32(ch5, priv->base_addr + REG5_CH5_OFFSET);

	// Write was succesful, so we return the number of bytes we wrote.
	return size;
}

static ssize_t ch6_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	u32 ch6;
	// Get the private adc data out of the dev struct
	struct adc_dev *priv = dev_get_drvdata(dev);

	ch6 = ioread32(priv->base_addr + REG6_CH6_OFFSET);

	return scnprintf(buf, PAGE_SIZE, "%u\n", ch6);
}

static ssize_t ch6_store(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t size)
{
	u32 ch6;
	int ret;
	struct adc_dev *priv = dev_get_drvdata(dev);

	// Parse the string we received as a u8
	// See https://elixir.bootlin.com/linux/latest/source/lib/kstrtox.c#L289 
	ret = kstrtou32(buf, 0, &ch6);
	if (ret < 0) {
		// kstrtou16 returned an error
		return ret;
	}

	iowrite32(ch6, priv->base_addr + REG6_CH6_OFFSET);

	// Write was succesful, so we return the number of bytes we wrote.
	return size;
}
static ssize_t ch7_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	u32 ch7;
	// Get the private adc data out of the dev struct
	struct adc_dev *priv = dev_get_drvdata(dev);

	ch7 = ioread32(priv->base_addr + REG7_CH7_OFFSET);

	return scnprintf(buf, PAGE_SIZE, "%u\n", ch7);
}

static ssize_t ch7_store(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t size)
{
	u32 ch7;
	int ret;
	struct adc_dev *priv = dev_get_drvdata(dev);

	// Parse the string we received as a u8
	// See https://elixir.bootlin.com/linux/latest/source/lib/kstrtox.c#L289 
	ret = kstrtou32(buf, 0, &ch7);
	if (ret < 0) {
		// kstrtou16 returned an error
		return ret;
	}

	iowrite32(ch7, priv->base_addr + REG7_CH7_OFFSET);

	// Write was succesful, so we return the number of bytes we wrote.
	return size;
}


/*-----------------------------------------------------------------------*/
/* sysfs Attributes                                                      */
/*-----------------------------------------------------------------------*/
// Define sysfs attributes
static DEVICE_ATTR_RW(ch0);
static DEVICE_ATTR_RW(ch1);
static DEVICE_ATTR_RW(ch2);
static DEVICE_ATTR_RW(ch3);
/* TODO: Add the attributes for REG1 and REG2 using register names       */
static DEVICE_ATTR_RW(ch4);   
static DEVICE_ATTR_RW(ch5);
static DEVICE_ATTR_RW(ch6);
static DEVICE_ATTR_RW(ch7);


// Create an atribute group so the device core can 
// export the attributes for us.
static struct attribute *adc_attrs[] = {
    &dev_attr_ch0.attr,
	&dev_attr_ch1.attr,
	&dev_attr_ch2.attr,
	&dev_attr_ch3.attr,
	&dev_attr_ch4.attr,

	&dev_attr_ch5.attr,
	&dev_attr_ch6.attr,
/* TODO: Add the attribute entries for REG1 and REG2 using register names*/
	&dev_attr_ch7.attr,

	NULL,
};
ATTRIBUTE_GROUPS(adc);


/*-----------------------------------------------------------------------*/
/* File Operations read()                                                */
/*-----------------------------------------------------------------------*/
/*
 * adc_read() - Read method for the adc char device
 * @file: Pointer to the char device file struct.
 * @buf: User-space buffer to read the value into.
 * @count: The number of bytes being requested.
 * @offset: The byte offset in the file being read from.
 *
 * Return: On success, the number of bytes written is returned and the
 * offset @offset is advanced by this number. On error, a negative error
 * value is returned.
 */
static ssize_t adc_read(struct file *file, char __user *buf,
	size_t count, loff_t *offset)
{
	size_t ret;
	u32 val;

	loff_t pos = *offset;

	/*
	 * Get the device's private data from the file struct's private_data
	 * field. The private_data field is equal to the miscdev field in the
	 * adc_dev struct. container_of returns the 
     * adc_dev struct that contains the miscdev in private_data.
	 */
	struct adc_dev *priv = container_of(file->private_data,
	                            struct adc_dev, miscdev);

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
		pr_warn("adc_read: unaligned access\n");
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
		pr_warn("adc_read: nothing copied\n");
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
 * adc_write() - Write method for the adc char device
 * @file: Pointer to the char device file struct.
 * @buf: User-space buffer to read the value from.
 * @count: The number of bytes being written.
 * @offset: The byte offset in the file being written to.
 *
 * Return: On success, the number of bytes written is returned and the
 * offset @offset is advanced by this number. On error, a negative error
 * value is returned.
 */
static ssize_t adc_write(struct file *file, const char __user *buf,
	size_t count, loff_t *offset)
{
	size_t ret;
	u32 val;

	loff_t pos = *offset;

	/*
	 * Get the device's private data from the file struct's private_data
	 * field. The private_data field is equal to the miscdev field in the
	 * adc_dev struct. container_of returns the 
     * adc_dev struct that contains the miscdev in private_data.
	 */
	struct adc_dev *priv = container_of(file->private_data,
	                              struct adc_dev, miscdev);

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
		pr_warn("adc_write: unaligned access\n");
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
		pr_warn("adc_write: nothing copied from user space\n");
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
 *  adc_fops - File operations supported by the  
 *                          adc driver
 * @owner: The adc driver owns the file operations; this 
 *         ensures that the driver can't be removed while the 
 *         character device is still in use.
 * @read: The read function.
 * @write: The write function.
 * @llseek: We use the kernel's default_llseek() function; this allows 
 *          users to change what position they are writing/reading to/from.
 */
static const struct file_operations  adc_fops = {
	.owner = THIS_MODULE,
	.read = adc_read,
	.write = adc_write,
	.llseek = default_llseek,
};


/*-----------------------------------------------------------------------*/
/* Platform Driver Probe (Initialization) Function                       */
/*-----------------------------------------------------------------------*/
/*
 * adc_probe() - Initialize device when a match is found
 * @pdev: Platform device structure associated with our 
 *        adc device; pdev is automatically created by the 
 *        driver core based upon our adc device tree node.
 *
 * When a device that is compatible with this adc driver 
 * is found, the driver's probe function is called. This probe function 
 * gets called by the kernel when an adc device is found 
 * in the device tree.
 */
static int adc_probe(struct platform_device *pdev)
{
	struct adc_dev *priv;
	int ret;

	/*
	 * Allocate kernel memory for the adc device and set it to 0.
	 * GFP_KERNEL specifies that we are allocating normal kernel RAM;
	 * see the kmalloc documentation for more info. The allocated memory
	 * is automatically freed when the device is removed.
	 */
	priv = devm_kzalloc(&pdev->dev, sizeof(struct adc_dev), GFP_KERNEL);
	if (!priv) {
		pr_err("Failed to allocate kernel memory for adc_led_pattern\n");
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
		pr_err("Failed to request/remap platform device resource (adc)\n");
		return PTR_ERR(priv->base_addr);
	}

	// Initialize the misc device parameters
	priv->miscdev.minor = MISC_DYNAMIC_MINOR;
	priv->miscdev.name = "adc";
	priv->miscdev.fops = &adc_fops;
	priv->miscdev.parent = &pdev->dev;
	priv->miscdev.groups = adc_groups;

	// Register the misc device; this creates a char dev at 
    // /dev/adc
	ret = misc_register(&priv->miscdev);
	if (ret) {
		pr_err("Failed to register misc device for adc\n");
		return ret;
	}

	// Attach the adc' private data to the 
    // platform device's struct.
	platform_set_drvdata(pdev, priv);

	pr_info("adc_probe successful\n");

	return 0;
}

/*-----------------------------------------------------------------------*/
/* Platform Driver Remove Function                                       */
/*-----------------------------------------------------------------------*/
/*
 * adc_remove() - Remove an adc device.
 * @pdev: Platform device structure associated with our adc device.
 *
 * This function is called when an adc devicee is removed or
 * the driver is removed.
 */
static int adc_remove(struct platform_device *pdev)
{
	// Get theadc' private data from the platform device.
	struct adc_dev *priv = platform_get_drvdata(pdev);

	// Deregister the misc device and remove the /dev/adc file.
	misc_deregister(&priv->miscdev);

	pr_info("adc_remove successful\n");

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
static const struct of_device_id adc_of_match[] = {
    // ****Note:**** This .compatible string must be identical to the 
    // .compatible string in the Device Tree Node for adc
	{ .compatible = "ajm,adc_0", },  
	{ }
};
MODULE_DEVICE_TABLE(of, adc_of_match);

/*-----------------------------------------------------------------------*/
/* Platform Driver Structure                                             */
/*-----------------------------------------------------------------------*/
/*
 * struct adc_driver - Platform driver struct for the 
 *                                  adc driver
 * @probe: Function that's called when a device is found
 * @remove: Function that's called when a device is removed
 * @driver.owner: Which module owns this driver
 * @driver.name: Name of the adc driver
 * @driver.of_match_table: Device tree match table
 * @driver.dev_groups: adc sysfs attribute group; this
 *                     allows the driver core to create the
 *                     attribute(s) without race conditions.
 */
static struct platform_driver adc_driver = {
	.probe = adc_probe,
	.remove = adc_remove,
	.driver = {
		.owner = THIS_MODULE,
		.name = "adc",
		.of_match_table = adc_of_match,
		.dev_groups = adc_groups,
	},
};

/*
 * We don't need to do anything special in module init/exit.
 * This macro automatically handles module init/exit.
 */
module_platform_driver(adc_driver);

MODULE_LICENSE("Dual MIT/GPL");
MODULE_AUTHOR("Anthony Meader");  // Adapted from Trevor Vannoy's Echo Driver
MODULE_DESCRIPTION("adc driver");
MODULE_VERSION("1.0");
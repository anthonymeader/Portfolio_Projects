/* SPDX-License-Identifier: GPL-2.0 or MIT                               */
/* Copyright(c) 2021 Ross K.Snider. All rights reserved.                 */
/*-------------------------------------------------------------------------
 * Description:  Linux Platform Device Driver for the 
 *               bandpassEQ component
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
#define REG0_passthrough_OFFSET 0x0
#define REG1_GAIN_1_OFFSET 0x04
#define REG2_GAIN_2_OFFSET 0x08
#define REG3_GAIN_3_OFFSET 0x0C
#define REG4_GAIN_4_OFFSET 0x10
#define REG5_GAIN_5_OFFSET 0x14
#define REG6_VOLUME_OFFSET 0x18

/* Memory span of all registers (used or not) in the                     */
/* component bandpassEQ                                            */
#define SPAN 0x20


/*-----------------------------------------------------------------------*/
/* bandpassEQ device structure                                     */
/*-----------------------------------------------------------------------*/
/*
 * struct  bandpassEQ_dev - Private bandpassEQ device struct.
 * @miscdev: miscdevice used to create a char device 
 *           for the bandpassEQ component
 * @base_addr: Base address of the bandpassEQ component
 * @lock: mutex used to prevent concurrent writes 
 *        to the bandpassEQ component
 *
 * An bandpassEQ_dev struct gets created for each bandpassEQ 
 * component in the system.
 */
struct bandpassEQ_dev {
	struct miscdevice miscdev;
	void __iomem *base_addr;
	struct mutex lock;
};

/*-----------------------------------------------------------------------*/
/* REG0: passthrough register read function show()                   */
/*-----------------------------------------------------------------------*/
/*
 * passthrough_show() - Return the passthrough value 
 *                          to user-space via sysfs.
 * @dev: Device structure for the bandpassEQ component. This 
 *       device struct is embedded in the bandpassEQ' device struct.
 * @attr: Unused.
 * @buf: Buffer that gets returned to user-space.
 *
 * Return: The number of bytes read.
 */
static ssize_t passthrough_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	u8 passthrough_reg;

	// Get the private bandpassEQ data out of the dev struct
	struct bandpassEQ_dev *priv = dev_get_drvdata(dev);

	passthrough_reg = ioread32(priv->base_addr + REG0_passthrough_OFFSET);

	return scnprintf(buf, PAGE_SIZE, "%u\n", passthrough_reg);
}
/*-----------------------------------------------------------------------*/
/* REG0: passthrough register write function store()                 */
/*-----------------------------------------------------------------------*/
/**
 * passthrough_store() - Store the passthrough value.
 * @dev: Device structure for the bandpassEQ component. This 
 *       device struct is embedded in the bandpassEQ' 
 *       platform device struct.
 * @attr: Unused.
 * @buf: Buffer that contains the passthrough value being written.
 * @size: The number of bytes being written.
 *
 * Return: The number of bytes stored.
 */
static ssize_t passthrough_store(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t size)
{
	u8 passthrough_reg;
	int ret;
	struct bandpassEQ_dev *priv = dev_get_drvdata(dev);

	// Parse the string we received as a bool
	// See https://elixir.bootlin.com/linux/latest/source/lib/kstrtox.c#L289 
	ret = kstrtou8(buf,0, &passthrough_reg);
	if (ret < 0) {
		// kstrtobool returned an error
		return ret;
	}

	iowrite32(passthrough_reg, priv->base_addr + REG0_passthrough_OFFSET);

	// Write was succesful, so we return the number of bytes we wrote.
	return size;
}


/*-----------------------------------------------------------------------*/
/* TODO: Add show() and store() functions for                            */
/* Registers REG1 (gain_1) and REG2 (BM)                    */
/* in component bandpassEQ                                         */
/*-----------------------------------------------------------------------*/
/* Add here...  */
/*
-----------------------
REG1 gain_1
-----------------------
*/
static ssize_t gain_1_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	u16 gain_1_reg;
	// Get the private bandpassEQ data out of the dev struct
	struct bandpassEQ_dev *priv = dev_get_drvdata(dev);

	gain_1_reg = ioread32(priv->base_addr + REG1_GAIN_1_OFFSET);

	return scnprintf(buf, PAGE_SIZE, "%u\n", gain_1_reg);
}

static ssize_t gain_1_store(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t size)
{
	u16 gain_1_reg;
	int ret;
	struct bandpassEQ_dev *priv = dev_get_drvdata(dev);

	// Parse the string we received as a u16
	// See https://elixir.bootlin.com/linux/latest/source/lib/kstrtox.c#L289 
	ret = kstrtou16(buf, 0, &gain_1_reg);
	if (ret < 0) {
		// kstrtou16 returned an error
		return ret;
	}

	iowrite32(gain_1_reg, priv->base_addr + REG1_GAIN_1_OFFSET);

	// Write was succesful, so we return the number of bytes we wrote.
	return size;
}
/*
-----------------------
REG2 BM
-----------------------
*/

static ssize_t gain_2_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	u16 gain_2_reg;
	// Get the private bandpassEQ data out of the dev struct
	struct bandpassEQ_dev *priv = dev_get_drvdata(dev);

	gain_2_reg = ioread32(priv->base_addr + REG2_GAIN_2_OFFSET);

	return scnprintf(buf, PAGE_SIZE, "%u\n", gain_2_reg);
}

static ssize_t gain_2_store(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t size)
{
	u16 gain_2_reg;
	int ret;
	struct bandpassEQ_dev *priv = dev_get_drvdata(dev);

	// Parse the string we received as a u16
	// See https://elixir.bootlin.com/linux/latest/source/lib/kstrtox.c#L289 
	ret = kstrtou16(buf, 0, &gain_2_reg);
	if (ret < 0) {
		// kstrtou16 returned an error
		return ret;
	}

	iowrite32(gain_2_reg, priv->base_addr + REG2_GAIN_2_OFFSET);

	// Write was succesful, so we return the number of bytes we wrote.
	return size;
}


/*-----------------------------------------------------------------------*/
/* REG3: WETDRYMIX register read function show()                           */
/*-----------------------------------------------------------------------*/

static ssize_t gain_3_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	u16 gain_3_reg;
	// Get the private bandpassEQ data out of the dev struct
	struct bandpassEQ_dev *priv = dev_get_drvdata(dev);

	gain_3_reg = ioread32(priv->base_addr + REG3_GAIN_3_OFFSET);

	return scnprintf(buf, PAGE_SIZE, "%u\n", gain_3_reg);
}

static ssize_t gain_3_store(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t size)
{
	u16 gain_3_reg;
	int ret;
	struct bandpassEQ_dev *priv = dev_get_drvdata(dev);

	// Parse the string we received as a u16
	// See https://elixir.bootlin.com/linux/latest/source/lib/kstrtox.c#L289 
	ret = kstrtou16(buf, 0, &gain_3_reg);
	if (ret < 0) {
		// kstrtou16 returned an error
		return ret;
	}

	iowrite32(gain_3_reg, priv->base_addr + REG3_GAIN_3_OFFSET);

	// Write was succesful, so we return the number of bytes we wrote.
	return size;
}
/*
 * WETDRYMIX_show() - Return the WETDRYMIX value to user-space via sysfs.
 * @dev: Device structure for the bandpassEQ component. This 
 *       device struct is embedded in the bandpassEQ' platform 
 *       device struct.
 * @attr: Unused.
 * @buf: Buffer that gets returned to user-space.
 *
 * Return: The number of bytes read.
 */

/*-----------------------------------------------------------------------*/
/* REG3: WETDRYMIX register write function store()                         */
/*-----------------------------------------------------------------------*/
static ssize_t gain_4_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	u16 gain_4_reg;
	// Get the private bandpassEQ data out of the dev struct
	struct bandpassEQ_dev *priv = dev_get_drvdata(dev);

	gain_4_reg = ioread32(priv->base_addr + REG4_GAIN_4_OFFSET);

	return scnprintf(buf, PAGE_SIZE, "%u\n", gain_4_reg);
}

static ssize_t gain_4_store(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t size)
{
	u16 gain_4_reg;
	int ret;
	struct bandpassEQ_dev *priv = dev_get_drvdata(dev);

	// Parse the string we received as a u16
	// See https://elixir.bootlin.com/linux/latest/source/lib/kstrtox.c#L289 
	ret = kstrtou16(buf, 0, &gain_4_reg);
	if (ret < 0) {
		// kstrtou16 returned an error
		return ret;
	}

	iowrite32(gain_4_reg, priv->base_addr + REG4_GAIN_4_OFFSET);

	// Write was succesful, so we return the number of bytes we wrote.
	return size;
}
/*
 * WETDRYMIX_store() - Store the WETDRYMIX value.
 * @dev: Device structure for the bandpassEQ component. This 
 *       device struct is embedded in the bandpassEQ' platform 
 *       device struct.
 * @attr: Unused.
 * @buf: Buffer that contains the  bandpassEQ value being written.
 * @size: The number of bytes being written.
 *
 * Return: The number of bytes stored.
 
 */
 static ssize_t gain_5_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	u16 gain_5_reg;
	// Get the private bandpassEQ data out of the dev struct
	struct bandpassEQ_dev *priv = dev_get_drvdata(dev);

	gain_5_reg = ioread32(priv->base_addr + REG5_GAIN_5_OFFSET);

	return scnprintf(buf, PAGE_SIZE, "%u\n", gain_5_reg);
}

static ssize_t gain_5_store(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t size)
{
	u16 gain_5_reg;
	int ret;
	struct bandpassEQ_dev *priv = dev_get_drvdata(dev);

	// Parse the string we received as a u16
	// See https://elixir.bootlin.com/linux/latest/source/lib/kstrtox.c#L289 
	ret = kstrtou16(buf, 0, &gain_5_reg);
	if (ret < 0) {
		// kstrtou16 returned an error
		return ret;
	}

	iowrite32(gain_5_reg, priv->base_addr + REG5_GAIN_5_OFFSET);

	// Write was succesful, so we return the number of bytes we wrote.
	return size;
}

//---------------
 static ssize_t volume_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	u16 volume_reg;
	// Get the private bandpassEQ data out of the dev struct
	struct bandpassEQ_dev *priv = dev_get_drvdata(dev);

	volume_reg = ioread32(priv->base_addr + REG6_VOLUME_OFFSET);

	return scnprintf(buf, PAGE_SIZE, "%u\n", volume_reg);
}

static ssize_t volume_store(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t size)
{
	u16 volume_reg;
	int ret;
	struct bandpassEQ_dev *priv = dev_get_drvdata(dev);

	// Parse the string we received as a u16
	// See https://elixir.bootlin.com/linux/latest/source/lib/kstrtox.c#L289 
	ret = kstrtou16(buf, 0, volume_reg);
	if (ret < 0) {
		// kstrtou16 returned an error
		return ret;
	}

	iowrite32(volume_reg, priv->base_addr + REG6_VOLUME_OFFSET);

	// Write was succesful, so we return the number of bytes we wrote.
	return size;
}


/*-----------------------------------------------------------------------*/
/* sysfs Attributes                                                      */
/*-----------------------------------------------------------------------*/
// Define sysfs attributes
static DEVICE_ATTR_RW(passthrough);    // Attribute for REG0
static DEVICE_ATTR_RW(gain_1);
static DEVICE_ATTR_RW(gain_2);
static DEVICE_ATTR_RW(gain_3);
static DEVICE_ATTR_RW(gain_4);
static DEVICE_ATTR_RW(gain_5);
static DEVICE_ATTR_RW(volume);




/* TODO: Add the attributes for REG1 and REG2 using register names       */

// Create an atribute group so the device core can 
// export the attributes for us.
static struct attribute *bandpassEQ_attrs[] = {
	&dev_attr_passthrough.attr,
	&dev_attr_gain_1.attr,
	&dev_attr_gain_2.attr,
	&dev_attr_gain_3.attr,
	&dev_attr_gain_4.attr,
	&dev_attr_gain_5.attr,
	&dev_attr_volume.attr,


/* TODO: Add the attribute entries for REG1 and REG2 using register names*/
	NULL,
};
ATTRIBUTE_GROUPS(bandpassEQ);


/*-----------------------------------------------------------------------*/
/* File Operations read()                                                */
/*-----------------------------------------------------------------------*/
/*
 * bandpassEQ_read() - Read method for the bandpassEQ char device
 * @file: Pointer to the char device file struct.
 * @buf: User-space buffer to read the value into.
 * @count: The number of bytes being requested.
 * @offset: The byte offset in the file being read from.
 *
 * Return: On success, the number of bytes written is returned and the
 * offset @offset is advanced by this number. On error, a negative error
 * value is returned.
 */
static ssize_t bandpassEQ_read(struct file *file, char __user *buf,
	size_t count, loff_t *offset)
{
	size_t ret;
	u32 val;

	loff_t pos = *offset;

	/*
	 * Get the device's private data from the file struct's private_data
	 * field. The private_data field is equal to the miscdev field in the
	 * bandpassEQ_dev struct. container_of returns the 
     * bandpassEQ_dev struct that contains the miscdev in private_data.
	 */
	struct bandpassEQ_dev *priv = container_of(file->private_data,
	                            struct bandpassEQ_dev, miscdev);

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
		pr_warn("bandpassEQ_read: unaligned access\n");
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
		pr_warn("bandpassEQ_read: nothing copied\n");
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
 * bandpassEQ_write() - Write method for the bandpassEQ char device
 * @file: Pointer to the char device file struct.
 * @buf: User-space buffer to read the value from.
 * @count: The number of bytes being written.
 * @offset: The byte offset in the file being written to.
 *
 * Return: On success, the number of bytes written is returned and the
 * offset @offset is advanced by this number. On error, a negative error
 * value is returned.
 */
static ssize_t bandpassEQ_write(struct file *file, const char __user *buf,
	size_t count, loff_t *offset)
{
	size_t ret;
	u32 val;

	loff_t pos = *offset;

	/*
	 * Get the device's private data from the file struct's private_data
	 * field. The private_data field is equal to the miscdev field in the
	 * bandpassEQ_dev struct. container_of returns the 
     * bandpassEQ_dev struct that contains the miscdev in private_data.
	 */
	struct bandpassEQ_dev *priv = container_of(file->private_data,
	                              struct bandpassEQ_dev, miscdev);

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
		pr_warn("bandpassEQ_write: unaligned access\n");
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
		pr_warn("bandpassEQ_write: nothing copied from user space\n");
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
 *  bandpassEQ_fops - File operations supported by the  
 *                          bandpassEQ driver
 * @owner: The bandpassEQ driver owns the file operations; this 
 *         ensures that the driver can't be removed while the 
 *         character device is still in use.
 * @read: The read function.
 * @write: The write function.
 * @llseek: We use the kernel's default_llseek() function; this allows 
 *          users to change what position they are writing/reading to/from.
 */
static const struct file_operations  bandpassEQ_fops = {
	.owner = THIS_MODULE,
	.read = bandpassEQ_read,
	.write = bandpassEQ_write,
	.llseek = default_llseek,
};


/*-----------------------------------------------------------------------*/
/* Platform Driver Probe (Initialization) Function                       */
/*-----------------------------------------------------------------------*/
/*
 * bandpassEQ_probe() - Initialize device when a match is found
 * @pdev: Platform device structure associated with our 
 *        bandpassEQ device; pdev is automatically created by the 
 *        driver core based upon our bandpassEQ device tree node.
 *
 * When a device that is compatible with this bandpassEQ driver 
 * is found, the driver's probe function is called. This probe function 
 * gets called by the kernel when an bandpassEQ device is found 
 * in the device tree.
 */
static int bandpassEQ_probe(struct platform_device *pdev)
{
	struct bandpassEQ_dev *priv;
	int ret;

	/*
	 * Allocate kernel memory for the bandpassEQ device and set it to 0.
	 * GFP_KERNEL specifies that we are allocating normal kernel RAM;
	 * see the kmalloc documentation for more info. The allocated memory
	 * is automatically freed when the device is removed.
	 */
	priv = devm_kzalloc(&pdev->dev, sizeof(struct bandpassEQ_dev), GFP_KERNEL);
	if (!priv) {
		pr_err("Failed to allocate kernel memory for bandpassEQ\n");
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
		pr_err("Failed to request/remap platform device resource (bandpassEQ)\n");
		return PTR_ERR(priv->base_addr);
	}

	// Initialize the misc device parameters
	priv->miscdev.minor = MISC_DYNAMIC_MINOR;
	priv->miscdev.name = "bandpassEQ";
	priv->miscdev.fops = &bandpassEQ_fops;
	priv->miscdev.parent = &pdev->dev;
	priv->miscdev.groups = bandpassEQ_groups;

	// Register the misc device; this creates a char dev at 
    // /dev/bandpassEQ
	ret = misc_register(&priv->miscdev);
	if (ret) {
		pr_err("Failed to register misc device for bandpassEQ\n");
		return ret;
	}

	// Attach the bandpassEQ' private data to the 
    // platform device's struct.
	platform_set_drvdata(pdev, priv);

	pr_info("bandpassEQ_probe successful\n");

	return 0;
}

/*-----------------------------------------------------------------------*/
/* Platform Driver Remove Function                                       */
/*-----------------------------------------------------------------------*/
/*
 * bandpassEQ_remove() - Remove an bandpassEQ device.
 * @pdev: Platform device structure associated with our bandpassEQ device.
 *
 * This function is called when an bandpassEQ devicee is removed or
 * the driver is removed.
 */
static int bandpassEQ_remove(struct platform_device *pdev)
{
	// Get thebandpassEQ' private data from the platform device.
	struct bandpassEQ_dev *priv = platform_get_drvdata(pdev);

	// Deregister the misc device and remove the /dev/bandpassEQ file.
	misc_deregister(&priv->miscdev);

	pr_info("bandpassEQ_remove successful\n");

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
static const struct of_device_id bandpassEQ_of_match[] = {
    // ****Note:**** This .compatible string must be identical to the 
    // .compatible string in the Device Tree Node for bandpassEQ
	{ .compatible = "ajm,bandpassEQ_0", },  
	{ }
};
MODULE_DEVICE_TABLE(of, bandpassEQ_of_match);

/*-----------------------------------------------------------------------*/
/* Platform Driver Structure                                             */
/*-----------------------------------------------------------------------*/
/*
 * struct bandpassEQ_driver - Platform driver struct for the 
 *                                  bandpassEQ driver
 * @probe: Function that's called when a device is found
 * @remove: Function that's called when a device is removed
 * @driver.owner: Which module owns this driver
 * @driver.name: Name of the bandpassEQ driver
 * @driver.of_match_table: Device tree match table
 * @driver.dev_groups: bandpassEQ sysfs attribute group; this
 *                     allows the driver core to create the
 *                     attribute(s) without race conditions.
 */
static struct platform_driver bandpassEQ_driver = {
	.probe = bandpassEQ_probe,
	.remove = bandpassEQ_remove,
	.driver = {
		.owner = THIS_MODULE,
		.name = "EQProcessor",
		.of_match_table = bandpassEQ_of_match,
		.dev_groups = bandpassEQ_groups,
	},
};

/*
 * We don't need to do anything special in module init/exit.
 * This macro automatically handles module init/exit.
 */
module_platform_driver(bandpassEQ_driver);

MODULE_LICENSE("Dual MIT/GPL");
MODULE_AUTHOR("Anthony Meader");  // Adapted from Trevor Vannoy's Echo Driver
MODULE_DESCRIPTION("EQProcessor driver");
MODULE_VERSION("1.0");


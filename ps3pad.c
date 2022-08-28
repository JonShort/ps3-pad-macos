//
// Copyright (c) 2021 vit9696.  All Rights Reserved.
// SPDX-License-Identifier: BSD-3-Clause
//
#include <IOKit/hid/IOHIDLib.h>
#include <CoreFoundation/CoreFoundation.h>
#include <stdio.h>
#include <stdlib.h>

static void activate_gamepad(void *device) {
  // Prepares the PS3 controller for our report, essentially makes it "listen"
  IOReturn openResult = IOHIDDeviceOpen(device, kIOHIDOptionsTypeNone);
  if (openResult != kIOReturnSuccess) {
    printf("  Failed to open DualShock 3 gamepad - %d\n", openResult);
    return;
  }

  printf("  Activating DualShock 3 gamepad...\n");
  // Our activation sequence - "wakes up" the controller
  uint8_t activateSequence[] = { 0x42, 0x0C, 0x00, 0x00 };

  // Here we actually send the sequence to the controller
  // 0xF4 is the report ID
  // docs - https://developer.apple.com/documentation/iokit/1588656-iohiddevicesetreport/
  IOHIDDeviceSetReport(device, kIOHIDReportTypeFeature, 0xF4, activateSequence, sizeof(activateSequence));

  // We're done communicating with this controller, so close the
  // connection, and stop the controller from listening
  IOHIDDeviceClose(device, kIOHIDOptionsTypeNone);
  printf(" Done!\n");
}

int main() {
  // You can see these values if you connect the controller, and check
  // System Information > USB
  static const SInt32 VendorId = 0x054C;
  static const SInt32 ProductId = 0x0268;

  // This is all required because IOHIDManagerSetDeviceMatching expects a CFDictionaryRef
  CFNumberRef vendorIdNum = CFNumberCreate(kCFAllocatorDefault, kCFNumberSInt32Type, &VendorId);
  CFNumberRef productIdNum = CFNumberCreate(kCFAllocatorDefault, kCFNumberSInt32Type, &ProductId);

  const void *keys[2] = {
    CFSTR(kIOHIDVendorIDKey),
    CFSTR(kIOHIDProductIDKey),
  };

  const void *values[2] = {
    vendorIdNum,
    productIdNum
  };

  IOHIDManagerRef manager = IOHIDManagerCreate(kCFAllocatorDefault, kIOHIDOptionsTypeNone);
  CFDictionaryRef matching = CFDictionaryCreate(NULL, keys, values, 2, NULL, NULL);
  // Effectively filters the list of all devices to just PS3 controllers,
  // so any operations using this manager from here on only have context of the PS3 pads
  // docs - https://developer.apple.com/documentation/iokit/1438371-iohidmanagersetdevicematching
  IOHIDManagerSetDeviceMatching(manager, matching);

  CFSetRef deviceSet = IOHIDManagerCopyDevices(manager);
  if (deviceSet != NULL) {
    CFIndex count = CFSetGetCount(deviceSet);

    if (count > 0) {
      printf("Discovered %ld DualShock 3 gamepads\n", count);

      void **gamepads = calloc(count, sizeof(void *));
      CFSetGetValues(deviceSet, (const void **)gamepads);

      for (CFIndex i = 0; i < count; i++) {
        printf(" Handling device %ld:\n", i);
        activate_gamepad(gamepads[i]);
      }

      free(gamepads);
    } else {
      printf("No DualShock 3 gamepads found!\n");
    }
    CFRelease(deviceSet);
  }

  CFRelease(productIdNum);
  CFRelease(vendorIdNum);
  CFRelease(matching);
  CFRelease(manager);
}

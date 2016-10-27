/* Copyright (c) 2013 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 *
 * Data structure and API definitions for a verified boot kernel image.
 * (Firmware Portion)
 */

#ifndef VBOOT_REFERENCE_VBOOT_KERNEL_H_
#define VBOOT_REFERENCE_VBOOT_KERNEL_H_

#include "cgptlib.h"
#include "gpt_misc.h"
#include "load_kernel_fw.h"
#include "vboot_api.h"

struct vb2_context;

/**
 * Exported for unit tests only - frees memory used by VbSelectAndLoadKernel()
 */
void VbApiKernelFree(VbCommonParams *cparams);

/**
 * Attempt loading a kernel from the specified type(s) of disks.
 *
 * If successful, sets p->disk_handle to the disk for the kernel and returns
 * VBERROR_SUCCESS.
 *
 * @param ctx			Vboot context
 * @param cparams		Vboot common params
 * @param p			Parameters for loading kernel
 * @param get_info_flags	Flags to pass to VbExDiskGetInfo()
 * @return VBERROR_SUCCESS, VBERROR_NO_DISK_FOUND if no disks of the specified
 * type were found, or other non-zero VBERROR_ codes for other failures.
 */
uint32_t VbTryLoadKernel(struct vb2_context *ctx, VbCommonParams *cparams,
			 LoadKernelParams *p,
                         uint32_t get_info_flags);

/* Flags for VbUserConfirms() */
#define VB_CONFIRM_MUST_TRUST_KEYBOARD (1 << 0)
#define VB_CONFIRM_SPACE_MEANS_NO      (1 << 1)

/**
 * Ask the user to confirm something.
 *
 * We should display whatever the question is first, then call this. ESC is
 * always "no", ENTER is always "yes", and we'll specify what SPACE means. We
 * don't return until one of those keys is pressed, or until asked to shut
 * down.
 *
 * Additionally, in some situations we don't accept confirmations from an
 * untrusted keyboard (such as a USB device).  In those cases, a recovery
 * button press is needed for confirmation, instead of ENTER.
 *
 * Returns: 1=yes, 0=no, -1 = shutdown.
 */
int VbUserConfirms(struct vb2_context *ctx, VbCommonParams *cparams,
		   uint32_t confirm_flags);

/**
 * Handle a normal boot.
 */
VbError_t VbBootNormal(struct vb2_context *ctx, VbCommonParams *cparams,
		       LoadKernelParams *p);

/**
 * Handle a developer-mode boot.
 */
VbError_t VbBootDeveloper(struct vb2_context *ctx, VbCommonParams *cparams,
			  LoadKernelParams *p);

/**
 * Handle a recovery-mode boot.
 */
VbError_t VbBootRecovery(struct vb2_context *ctx, VbCommonParams *cparams,
			 LoadKernelParams *p);

/**
 * Sync EC device <devidx> firmware to expected version.
 *
 * @param ctx		Vboot context
 * @param devidx	EC device index to sync
 * @param cparams	Common vboot params
 */
VbError_t VbEcSoftwareSync(struct vb2_context *ctx, int devidx,
			   VbCommonParams *cparams);

#endif  /* VBOOT_REFERENCE_VBOOT_KERNEL_H_ */

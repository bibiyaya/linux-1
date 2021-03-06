/*
 * vsp1_rwpf.h  --  R-Car VSP1 Read and Write Pixel Formatters
 *
 * Copyright (C) 2013-2014 Renesas Electronics Corporation
 *
 * Contact: Laurent Pinchart (laurent.pinchart@ideasonboard.com)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#ifndef __VSP1_RWPF_H__
#define __VSP1_RWPF_H__

#include <linux/spinlock.h>

#include <media/media-entity.h>
#include <media/v4l2-ctrls.h>
#include <media/v4l2-subdev.h>

#include "vsp1.h"
#include "vsp1_entity.h"

#define RWPF_PAD_SINK				0
#define RWPF_PAD_SOURCE				1

struct v4l2_ctrl;
struct vsp1_dl_manager;
struct vsp1_pipeline;
struct vsp1_rwpf;
struct vsp1_video;

struct vsp1_rwpf_memory {
	dma_addr_t addr[3];
};

struct vsp1_rwpf {
	struct vsp1_entity entity;
	struct v4l2_ctrl_handler ctrls;

	struct vsp1_pipeline *pipe;
	struct vsp1_video *video;

	unsigned int max_width;
	unsigned int max_height;

	struct v4l2_pix_format_mplane format;
	const struct vsp1_format_info *fmtinfo;
	unsigned int bru_input;

	unsigned int alpha;

	u32 mult_alpha;
	u32 outfmt;

	struct {
		spinlock_t lock;
		struct v4l2_ctrl *ctrls[2];
		unsigned int pending;
		unsigned int active;
	} flip;

	unsigned int offsets[2];
	struct vsp1_rwpf_memory mem;

	struct vsp1_dl_manager *dlm;
};

static inline struct vsp1_rwpf *to_rwpf(struct v4l2_subdev *subdev)
{
	return container_of(subdev, struct vsp1_rwpf, entity.subdev);
}

static inline struct vsp1_rwpf *entity_to_rwpf(struct vsp1_entity *entity)
{
	return container_of(entity, struct vsp1_rwpf, entity);
}

struct vsp1_rwpf *vsp1_rpf_create(struct vsp1_device *vsp1, unsigned int index);
struct vsp1_rwpf *vsp1_wpf_create(struct vsp1_device *vsp1, unsigned int index);

int vsp1_rwpf_init_ctrls(struct vsp1_rwpf *rwpf, unsigned int ncontrols);

extern const struct v4l2_subdev_pad_ops vsp1_rwpf_pad_ops;

struct v4l2_rect *vsp1_rwpf_get_crop(struct vsp1_rwpf *rwpf,
				     struct v4l2_subdev_pad_config *config);
/**
 * vsp1_rwpf_set_memory - Configure DMA addresses for a [RW]PF
 * @rwpf: the [RW]PF instance
 * @dl: the display list
 *
 * This function applies the cached memory buffer address to the display list.
 */
static inline void vsp1_rwpf_set_memory(struct vsp1_rwpf *rwpf,
					struct vsp1_dl_list *dl)
{
	rwpf->entity.ops->set_memory(&rwpf->entity, dl);
}

#endif /* __VSP1_RWPF_H__ */

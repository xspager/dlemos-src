/*

by xspager@gmail.com
*/
package main

import (
	"os"
	"syscall"
	"log"
	"unsafe"
	"time"
)

const (
	FBIOGET_VSCREENINFO	= 0x4600
	FBIOGET_FSCREENINFO = 0x4602
)

const (
	FB_TYPE_PACKED_PIXELS         = 0       /* Packed Pixels        */
	FB_TYPE_PLANES                = 1       /* Non interleaved planes */
	FB_TYPE_INTERLEAVED_PLANES    = 2       /* Interleaved planes   */
	FB_TYPE_TEXT                  = 3       /* Text/attributes      */
	FB_TYPE_VGA_PLANES            = 4       /* EGA/VGA planes       */
)

const (
	FB_VMODE_NONINTERLACED = 0       /* non interlaced */
	FB_VMODE_INTERLACED    = 1       /* interlaced   */
	FB_VMODE_DOUBLE        = 2       /* double scan */
	FB_VMODE_ODD_FLD_FIRST = 4       /* interlaced: top line first */
	FB_VMODE_MASK          = 255
	FB_VMODE_YWRAP         = 256     /* ywrap instead of panning     */
	FB_VMODE_SMOOTH_XPAN   = 512     /* smooth xpan possible (internally used) */
//	FB_VMODE_CONUPDATE     = 512     /* don't update x/yoffset       */
)

type fb_bitfield struct {
	offset uint32              /* beginning of bitfield        */
    length uint32              /* length of bitfield           */
    msb_right uint32
}

type fb_fix_screeninfo struct {
        id[16] uint8                    /* identification string eg "TT Builtin" */
        smem_start uint32               /* Start of frame buffer mem */
                                        /* (physical address) */
        smem_len,                       /* Length of frame buffer mem */
        _type,                           /* see FB_TYPE_*                */
        type_aux,                       /* Interleave for interleaved Planes */
        visual uint32                   /* see FB_VISUAL_*              */
        xpanstep,                       /* zero if no hardware panning  */
        ypanstep,                       /* zero if no hardware panning  */
        ywrapstep uint16                /* zero if no hardware ywrap    */
        line_length uint32              /* length of a line in bytes    */
        mmio_start uint32               /* Start of Memory Mapped I/O   */
                                        /* (physical address) */
        mmio_len,                       /* Length of Memory Mapped I/O  */
        accel uint32                    /* Indicate to driver which     */
                                        /*  specific chip/card we have  */
        reserved[3] uint16              /* Reserved for future compatibility */
}


type fb_var_screeninfo struct {
	xres,                     /* visible resolution           */
    yres,
    xres_virtual,             /* virtual resolution           */
    yres_virtual,
    xoffset,                  /* offset from virtual to visible */
    yoffset,                  /* resolution                   */

    bits_per_pixel,           /* guess what                   */
    grayscale uint32          /* != 0 Graylevels instead of colors */

    red fb_bitfield           /* bitfield in fb mem if true color, */
    green fb_bitfield         /* else only length is significant */
    blue fb_bitfield
    transp fb_bitfield        /* transparency                 */

    nonstd,                   /* != 0 Non standard pixel format */

    activate,                 /* see FB_ACTIVATE_*            */

    height,                   /* height of picture in mm    */
    width,                    /* width of picture in mm     */

    accel_flags,              /* (OBSOLETE) see fb_info.flags */

    /* Timing: All values in pixclocks, except pixclock (of course) */
    pixclock,                 /* pixel clock in ps (pico seconds) */
    left_margin,              /* time from sync to picture    */
    right_margin,             /* time from picture to sync    */
    upper_margin,             /* time from sync to picture    */
    lower_margin,
    hsync_len,                /* length of horizontal sync    */
    vsync_len,                /* length of vertical sync      */
    sync,                     /* see FB_SYNC_*                */
    vmode,                    /* see FB_VMODE_*               */
    rotate uint32             /* angle we rotate counter clockwise */
    reserved[5] uint32;       /* Reserved for future compatibility */
}

func main() {

	const (
		fbdev = "/dev/fb0"
		//fbdev = "/dev/graphics/fb0"
	)

	var (
		scrinfo fb_var_screeninfo
		fixedinfo fb_fix_screeninfo
	)

	f, err := os.Open(fbdev, os.O_RDWR, 0)

	if err != nil {
		log.Exitf(err.String())
		return
	} else {
		log.Printf("Opened %s", fbdev)
	}

	fd := f.Fd();

	_, _, errno := syscall.Syscall(syscall.SYS_IOCTL, uintptr(fd), uintptr(FBIOGET_VSCREENINFO), uintptr(unsafe.Pointer(&scrinfo)))

	if errno != 0 {
		log.Exitf("ioctl(FBIOGET_VSCREENINFO): %s", os.Errno(errno))
	}

	_, _, errno = syscall.Syscall(syscall.SYS_IOCTL, uintptr(fd), uintptr(FBIOGET_FSCREENINFO), uintptr(unsafe.Pointer(&fixedinfo)))

	if errno != 0 {
		log.Exitf("ioctl(FBIOGET_FSCREENINFO): %s", os.Errno(errno))
	}

//	buflen := scrinfo.yres_virtual * fixedinfo.line_length

	log.Printf("FB Driver: %s", fixedinfo.id)
	log.Printf("Size: %dkb", fixedinfo.smem_len / 1024)
	log.Printf("Resolution: %d x %d x %d", scrinfo.xres, scrinfo.yres, scrinfo.bits_per_pixel)

	var fb_type = "unkonwn"

	switch fixedinfo._type {
		case FB_TYPE_PACKED_PIXELS:
			fb_type = "packed pixels"
		case FB_TYPE_PLANES:
			fb_type = "planes"
		case FB_TYPE_INTERLEAVED_PLANES:
			fb_type = "interleaved"
		case FB_TYPE_TEXT:
			fb_type = "text"
		case FB_TYPE_VGA_PLANES:
			fb_type = "vga planes"
	}

	log.Printf("Type: %s", fb_type)

	var fb_mode = "unkonwn"

	switch scrinfo.vmode {
		case FB_VMODE_NONINTERLACED: fb_mode = "non interlaced"
		case FB_VMODE_INTERLACED: fb_mode = "interlaced"
		case FB_VMODE_DOUBLE: fb_mode = "double buffered"
		case FB_VMODE_ODD_FLD_FIRST: fb_mode = "odd fld first"
		case FB_VMODE_MASK: fb_mode = "mask"
		case FB_VMODE_YWRAP: fb_mode = "ywrap"
		case FB_VMODE_SMOOTH_XPAN: fb_mode = "smooth xpan or con update"
		//case FB_VMODE_CONUPDATE: fb_mode = "con update"
	}

	log.Printf("Mode: %s", fb_mode)

	if scrinfo.nonstd != 0 {
		log.Printf("Non standard pixel format")
	}
	log.Printf("\t Red: %dbits, Green: %dbits, Blue: %dbits, Transparence: %dbits", scrinfo.red.length, scrinfo.green.length, scrinfo.blue.length, scrinfo.transp.length)

	log.Printf("Red:\n\tOffset: %d", scrinfo.red.offset)
	log.Printf("Green:\n\tOffset: %d", scrinfo.green.offset)
	log.Printf("Blue:\n\tOffset: %d", scrinfo.blue.offset)
	log.Printf("Transparence:\n\tOffset: %d", scrinfo.transp.offset)



	addr, _, errno := syscall.Syscall6(syscall.SYS_MMAP2,
		0,
		uintptr(640*480),
		/*syscall.PROT_READ*/ 1 | /*syscall.PROT_WRITE*/ 2,
		/*syscall.MAP_SHARED*/ 1,
		uintptr(fd),
		0)

	if errno != 0 {
		log.Exitf("mmap display: %s\n", os.Errno(errno))
	}

	var (
		width = int(scrinfo.yres)
		height = int(scrinfo.xres)
	)

	mem := (*byte[])(unsafe.Pointer(addr))

	for x := 0; x < height; x++ {
		for y := 0; y < width; y++ {
			mem[y+(x*width)] = 10
			mem[y+(x*width)] = 10
			mem[y+(x*width)] = 10
			mem[y+(x*width)] = 10
		}
	}

	time.Sleep(500000)

	f.Close()
}

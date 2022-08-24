#include "images.h"

/* This artwork was made by Markus Persson in December 2011 */
const u8 level_tileset[32 * 128] = {
    // generic ground 0
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x21, 0x11, 0x11,
    0x11, 0x11, 0x12, 0x11,
    0x12, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,

    // generic ground 1
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x12, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x12, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,

    // generic ground 2
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x21, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x21,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x12, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,

    // generic ground 3
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x12, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x21, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,

    // -----

    // unconnected grass/sand top-left
    0xff, 0xff, 0xff, 0x22,
    0xff, 0xf2, 0x22, 0x11,
    0xff, 0x21, 0x11, 0x11,
    0xf2, 0x11, 0x11, 0x11,
    0xf2, 0x11, 0x11, 0x11,
    0xf2, 0x11, 0x11, 0x11,
    0x21, 0x11, 0x11, 0x11,
    0x21, 0x11, 0x11, 0x11,

    // unconnected grass/sand top-right
    0x22, 0xff, 0xff, 0xff,
    0x11, 0x22, 0x2f, 0xff,
    0x11, 0x11, 0x11, 0xff,
    0x11, 0x11, 0x11, 0x3f,
    0x11, 0x11, 0x11, 0x3f,
    0x11, 0x11, 0x11, 0x3f,
    0x11, 0x11, 0x11, 0x13,
    0x11, 0x11, 0x11, 0x13,

    // unconnected grass/sand bottom-left
    0x21, 0x11, 0x11, 0x11,
    0x21, 0x11, 0x11, 0x11,
    0xf2, 0x11, 0x11, 0x11,
    0xf2, 0x11, 0x11, 0x11,
    0xf2, 0x11, 0x11, 0x11,
    0xff, 0x11, 0x11, 0x11,
    0xff, 0xf3, 0x33, 0x11,
    0xff, 0xff, 0xff, 0x33,

    // unconnected grass/sand bottom-right
    0x11, 0x11, 0x11, 0x13,
    0x11, 0x11, 0x11, 0x13,
    0x11, 0x11, 0x11, 0x3f,
    0x11, 0x11, 0x11, 0x3f,
    0x11, 0x11, 0x11, 0x3f,
    0x11, 0x11, 0x13, 0xff,
    0x11, 0x33, 0x3f, 0xff,
    0x33, 0xff, 0xff, 0xff,

    // unconnected grass/sand top
    0x22, 0xff, 0xff, 0x22,
    0x11, 0x22, 0x22, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,

    // unconnected grass/sand right
    0x11, 0x11, 0x11, 0x13,
    0x11, 0x11, 0x11, 0x13,
    0x11, 0x11, 0x11, 0x3f,
    0x11, 0x11, 0x11, 0x3f,
    0x11, 0x11, 0x11, 0x3f,
    0x11, 0x11, 0x11, 0x3f,
    0x11, 0x11, 0x11, 0x13,
    0x11, 0x11, 0x11, 0x13,

    // unconnected grass/sand bottom
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x33, 0x33, 0x11,
    0x33, 0xff, 0xff, 0x33,

    // unconnected grass/sand left
    0x21, 0x11, 0x11, 0x11,
    0x21, 0x11, 0x11, 0x11,
    0xf2, 0x11, 0x11, 0x11,
    0xf2, 0x11, 0x11, 0x11,
    0xf2, 0x11, 0x11, 0x11,
    0xf2, 0x11, 0x11, 0x11,
    0x21, 0x11, 0x11, 0x11,
    0x21, 0x11, 0x11, 0x11,

    // -----
    // the rock tiles have transparent background: in all levels
    // but sky the background will be the dirt color
    //
    // this makes it possible for the cloud to use these tiles

    // unconnected rock/cloud top-left
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x33,
    0x00, 0x03, 0x33, 0x44,
    0x00, 0x34, 0x44, 0x43,
    0x03, 0x44, 0x43, 0x31,
    0x03, 0x44, 0x31, 0x11,
    0x34, 0x43, 0x11, 0x11,
    0x34, 0x43, 0x11, 0x11,

    // unconnected rock/cloud top-right
    0x00, 0x00, 0x00, 0x00,
    0x33, 0x30, 0x00, 0x00,
    0x44, 0x43, 0x30, 0x00,
    0x33, 0x44, 0x43, 0x00,
    0x11, 0x34, 0x33, 0x30,
    0x11, 0x13, 0x33, 0x30,
    0x11, 0x13, 0x33, 0x30,
    0x11, 0x11, 0x33, 0x30,

    // unconnected rock/cloud bottom-left
    0x34, 0x44, 0x33, 0x11,
    0x34, 0x43, 0x33, 0x33,
    0x03, 0x43, 0x33, 0x33,
    0x03, 0x33, 0x33, 0x33,
    0x03, 0x33, 0x33, 0x33,
    0x00, 0x33, 0x33, 0x33,
    0x00, 0x03, 0x33, 0x33,
    0x00, 0x00, 0x00, 0x33,

    // unconnected rock/cloud bottom-right
    0x11, 0x33, 0x33, 0x33,
    0x33, 0x33, 0x33, 0x33,
    0x33, 0x33, 0x33, 0x30,
    0x33, 0x33, 0x33, 0x30,
    0x33, 0x33, 0x33, 0x30,
    0x33, 0x33, 0x33, 0x00,
    0x33, 0x33, 0x30, 0x00,
    0x33, 0x00, 0x00, 0x00,

    // unconnected rock/cloud top
    0x00, 0x00, 0x00, 0x00,
    0x33, 0x30, 0x00, 0x33,
    0x44, 0x43, 0x33, 0x44,
    0x33, 0x44, 0x44, 0x43,
    0x11, 0x33, 0x33, 0x31,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,

    // unconnected rock/cloud right
    0x11, 0x11, 0x33, 0x33,
    0x11, 0x11, 0x33, 0x33,
    0x11, 0x13, 0x33, 0x30,
    0x11, 0x13, 0x33, 0x30,
    0x11, 0x13, 0x33, 0x30,
    0x11, 0x13, 0x33, 0x30,
    0x11, 0x11, 0x33, 0x33,
    0x11, 0x11, 0x33, 0x33,

    // unconnected rock/cloud bottom
    0x11, 0x33, 0x33, 0x11,
    0x33, 0x33, 0x33, 0x33,
    0x33, 0x33, 0x33, 0x33,
    0x33, 0x33, 0x33, 0x33,
    0x33, 0x33, 0x33, 0x33,
    0x33, 0x33, 0x33, 0x33,
    0x33, 0x33, 0x33, 0x33,
    0x33, 0x00, 0x00, 0x33,

    // unconnected rock/cloud left
    0x34, 0x43, 0x11, 0x11,
    0x34, 0x43, 0x11, 0x11,
    0x03, 0x44, 0x31, 0x11,
    0x03, 0x44, 0x31, 0x11,
    0x03, 0x44, 0x31, 0x11,
    0x03, 0x44, 0x31, 0x11,
    0x34, 0x43, 0x11, 0x11,
    0x34, 0x43, 0x11, 0x11,

    // rock/cloud corner 0
    0x34, 0x43, 0x11, 0x11,
    0x34, 0x43, 0x11, 0x11,
    0x44, 0x31, 0x11, 0x11,
    0x33, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,

    // rock/cloud corner 1
    0x11, 0x11, 0x33, 0x33,
    0x11, 0x11, 0x33, 0x33,
    0x11, 0x11, 0x13, 0x34,
    0x11, 0x11, 0x11, 0x33,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,

    // rock/cloud corner 2
    0x33, 0x31, 0x11, 0x11,
    0x33, 0x33, 0x11, 0x11,
    0x33, 0x34, 0x31, 0x11,
    0x33, 0x34, 0x31, 0x11,
    0x33, 0x44, 0x31, 0x11,
    0x33, 0x43, 0x11, 0x11,
    0x34, 0x43, 0x11, 0x11,
    0x34, 0x43, 0x11, 0x11,

    // rock/cloud corner 3
    0x11, 0x11, 0x13, 0x33,
    0x11, 0x11, 0x33, 0x33,
    0x11, 0x13, 0x33, 0x33,
    0x11, 0x13, 0x33, 0x33,
    0x11, 0x13, 0x33, 0x33,
    0x11, 0x11, 0x33, 0x33,
    0x11, 0x11, 0x33, 0x33,
    0x11, 0x11, 0x33, 0x33,

    // -----

    // unconnected hole top-left
    0xff, 0xff, 0xff, 0xee,
    0xff, 0xfe, 0xee, 0xee,
    0xff, 0xee, 0xee, 0x11,
    0xfe, 0xee, 0x11, 0x11,
    0xfe, 0xe1, 0x11, 0x11,
    0xfe, 0xe1, 0x11, 0x11,
    0xee, 0x11, 0x11, 0x11,
    0xee, 0x11, 0x11, 0x11,

    // unconnected hole top-right
    0xee, 0xff, 0xff, 0xff,
    0xee, 0xee, 0xef, 0xff,
    0x11, 0xee, 0xee, 0xff,
    0x11, 0x11, 0xee, 0xef,
    0x11, 0x11, 0x1e, 0xef,
    0x11, 0x11, 0x1e, 0xef,
    0x11, 0x11, 0x11, 0xee,
    0x11, 0x11, 0x11, 0xee,

    // unconnected hole bottom-left
    0xee, 0x11, 0x11, 0x11,
    0xee, 0x11, 0x11, 0x11,
    0xfe, 0x11, 0x11, 0x11,
    0xfe, 0xe1, 0x11, 0x11,
    0xfe, 0xe1, 0x11, 0x11,
    0xff, 0xee, 0x11, 0x11,
    0xff, 0xfe, 0xee, 0x11,
    0xff, 0xff, 0xff, 0xee,

    // unconnected hole bottom-right
    0x11, 0x11, 0x11, 0xee,
    0x11, 0x11, 0x11, 0xee,
    0x11, 0x11, 0x11, 0xef,
    0x11, 0x11, 0x1e, 0xef,
    0x11, 0x11, 0x1e, 0xef,
    0x11, 0x11, 0xee, 0xff,
    0x11, 0xee, 0xef, 0xff,
    0xee, 0xff, 0xff, 0xff,

    // unconnected hole top
    0xee, 0xff, 0xff, 0xee,
    0xee, 0xee, 0xee, 0xee,
    0x11, 0xee, 0xee, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,

    // unconnected hole right
    0x11, 0x11, 0x11, 0xee,
    0x11, 0x11, 0x11, 0xee,
    0x11, 0x11, 0x11, 0xef,
    0x11, 0x11, 0x1e, 0xef,
    0x11, 0x11, 0x1e, 0xef,
    0x11, 0x11, 0x1e, 0xef,
    0x11, 0x11, 0x1e, 0xee,
    0x11, 0x11, 0x11, 0xee,

    // unconnected hole bottom
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0xee, 0xee, 0x11,
    0xee, 0xff, 0xff, 0xee,

    // unconnected hole left
    0xee, 0x11, 0x11, 0x11,
    0xee, 0x11, 0x11, 0x11,
    0xfe, 0x11, 0x11, 0x11,
    0xfe, 0xe1, 0x11, 0x11,
    0xfe, 0xe1, 0x11, 0x11,
    0xfe, 0xe1, 0x11, 0x11,
    0xee, 0xe1, 0x11, 0x11,
    0xee, 0x11, 0x11, 0x11,

    // -----

    // fully transparent
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,

    // -----

    // flower
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x14, 0x44, 0x11,
    0x11, 0x14, 0x44, 0x11,
    0x14, 0x45, 0x55, 0x44,
    0x14, 0x45, 0x55, 0x44,
    0x16, 0x64, 0x44, 0x66,
    0x11, 0x14, 0x44, 0x11,
    0x11, 0x16, 0x66, 0x11,


    // -----

    // single tree top-left
    0x11, 0x11, 0x11, 0x66,
    0x11, 0x11, 0x66, 0x33,
    0x11, 0x16, 0x33, 0x33,
    0x11, 0x63, 0x37, 0x73,
    0x11, 0x63, 0x77, 0x33,
    0x16, 0x33, 0x77, 0x33,
    0x16, 0x33, 0x77, 0x33,
    0x16, 0x33, 0x33, 0x33,

    // single tree top-right
    0x66, 0x11, 0x11, 0x11,
    0x33, 0x66, 0x11, 0x11,
    0x33, 0x33, 0x61, 0x11,
    0x33, 0x33, 0x36, 0x11,
    0x33, 0x33, 0x36, 0x11,
    0x33, 0x33, 0x33, 0x61,
    0x33, 0x33, 0x33, 0x61,
    0x33, 0x33, 0x33, 0x61,

    // single tree bottom-left
    0x16, 0x33, 0x33, 0x33,
    0x63, 0x33, 0x33, 0x33,
    0x63, 0x63, 0x33, 0x33,
    0x16, 0x16, 0x33, 0x63,
    0x11, 0x11, 0x66, 0x86,
    0x11, 0x11, 0x68, 0x88,
    0x11, 0x11, 0x68, 0x88,
    0x11, 0x11, 0x16, 0x66,

    // single tree bottom-right
    0x33, 0x33, 0x33, 0x61,
    0x33, 0x33, 0x33, 0x36,
    0x33, 0x33, 0x36, 0x36,
    0x36, 0x33, 0x61, 0x61,
    0x69, 0x66, 0x61, 0x11,
    0x99, 0x96, 0x11, 0x11,
    0x99, 0x96, 0x11, 0x11,
    0x66, 0x61, 0x11, 0x11,

    // tree group 0
    0x33, 0x33, 0x33, 0x66,
    0x33, 0x33, 0x66, 0x33,
    0x33, 0x36, 0x33, 0x33,
    0x33, 0x63, 0x37, 0x73,
    0x66, 0x63, 0x77, 0x33,
    0x66, 0x33, 0x77, 0x33,
    0x66, 0x33, 0x77, 0x33,
    0x66, 0x33, 0x33, 0x33,

    // tree group 1
    0x66, 0x33, 0x33, 0x33,
    0x33, 0x66, 0x33, 0x33,
    0x33, 0x33, 0x63, 0x33,
    0x33, 0x33, 0x36, 0x33,
    0x33, 0x33, 0x36, 0x66,
    0x33, 0x33, 0x33, 0x69,
    0x33, 0x33, 0x33, 0x69,
    0x33, 0x33, 0x33, 0x66,

    // -----

    // dirt 0
    0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff,
    0xff, 0xef, 0xff, 0xff,
    0xff, 0xff, 0xfe, 0xff,
    0xfe, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff,

    // dirt 1
    0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff,
    0xff, 0xfe, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xfe, 0xff,
    0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff,

    // dirt 2
    0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff,
    0xff, 0xef, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xef,
    0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff,
    0xff, 0xfe, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff,

    // dirt 3
    0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xfe, 0xff,
    0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff,
    0xff, 0xef, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff,

    // -----

    // sand stepped on
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x12, 0x21, 0x11,
    0x11, 0x21, 0x14, 0x11,
    0x11, 0x21, 0x14, 0x11,
    0x11, 0x14, 0x41, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,

    // -----

    // cactus top-left
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x55,
    0x11, 0x11, 0x55, 0xbb,
    0x11, 0x15, 0xbb, 0xb5,
    0x11, 0x15, 0xbb, 0xbb,
    0x11, 0x55, 0xb5, 0xbb,
    0x11, 0x15, 0xba, 0xbb,
    0x11, 0x15, 0xbb, 0xbb,

    // cactus top-right
    0x51, 0x11, 0x11, 0x11,
    0x55, 0x11, 0x11, 0x11,
    0xba, 0x55, 0x11, 0x11,
    0xbb, 0xaa, 0x51, 0x11,
    0xbb, 0x5a, 0x55, 0x11,
    0xbb, 0xaa, 0x51, 0x11,
    0xb5, 0xaa, 0x55, 0x11,
    0xba, 0xaa, 0x51, 0x11,

    // cactus bottom-left
    0x11, 0x15, 0xbb, 0xb5,
    0x11, 0x15, 0xbb, 0xba,
    0x11, 0x55, 0xbb, 0xbb,
    0x11, 0x15, 0xbb, 0xbb,
    0x11, 0x15, 0xbb, 0x5b,
    0x11, 0x15, 0xbb, 0xab,
    0x11, 0x11, 0x55, 0xbb,
    0x11, 0x11, 0x11, 0x55,

    // cactus bottom-right
    0xbb, 0xaa, 0x51, 0x11,
    0xbb, 0xaa, 0x51, 0x11,
    0xbb, 0xaa, 0x55, 0x11,
    0xbb, 0x5a, 0x51, 0x11,
    0xbb, 0xaa, 0x51, 0x11,
    0xbb, 0xaa, 0x51, 0x11,
    0xba, 0x55, 0x11, 0x11,
    0x55, 0x11, 0x11, 0x11,

    // -----

    // farmland
    0x44, 0x55, 0x55, 0x54,
    0x55, 0x44, 0x44, 0x45,
    0x00, 0x00, 0x40, 0x00,
    0x55, 0x45, 0x55, 0x55,
    0x44, 0x40, 0x04, 0x44,
    0x40, 0x05, 0x50, 0x00,
    0x54, 0x54, 0x45, 0x55,
    0x40, 0x00, 0x00, 0x04,

    // wheat 0
    0x44, 0x55, 0x55, 0x54,
    0x55, 0x64, 0x44, 0x45,
    0x00, 0x00, 0x40, 0x00,
    0x55, 0x45, 0x55, 0x55,
    0x44, 0x40, 0x04, 0x64,
    0x40, 0x65, 0x50, 0x00,
    0x54, 0x54, 0x45, 0x55,
    0x40, 0x00, 0x60, 0x04,

    // wheat 1
    0x44, 0x65, 0x55, 0x54,
    0x55, 0x64, 0x44, 0x65,
    0x00, 0x00, 0x40, 0x00,
    0x55, 0x45, 0x55, 0x65,
    0x46, 0x40, 0x04, 0x64,
    0x40, 0x65, 0x50, 0x00,
    0x54, 0x54, 0x65, 0x55,
    0x40, 0x00, 0x60, 0x04,

    // wheat 2
    0x44, 0x65, 0x65, 0x64,
    0x56, 0x64, 0x44, 0x65,
    0x00, 0x00, 0x60, 0x00,
    0x56, 0x45, 0x65, 0x65,
    0x46, 0x46, 0x04, 0x64,
    0x60, 0x65, 0x60, 0x06,
    0x64, 0x54, 0x65, 0x56,
    0x40, 0x60, 0x60, 0x04,

    // wheat 3
    0x47, 0x84, 0x78, 0x74,
    0x77, 0x87, 0x84, 0x78,
    0x78, 0x47, 0x78, 0x44,
    0x77, 0x84, 0x78, 0x78,
    0x47, 0x87, 0x87, 0x78,
    0x78, 0x78, 0x47, 0x87,
    0x78, 0x78, 0x78, 0x87,
    0x44, 0x78, 0x78, 0x44,

    // wheat 4
    0x49, 0xa4, 0x9a, 0x94,
    0x99, 0xa9, 0xa4, 0x9a,
    0x9a, 0x49, 0x9a, 0x44,
    0x99, 0xa4, 0x9a, 0x9a,
    0x49, 0xa9, 0xa9, 0x9a,
    0x9a, 0x9a, 0x49, 0xa9,
    0x9a, 0x9a, 0x9a, 0xa9,
    0x44, 0x9a, 0x9a, 0x44,

    // wheat 5
    0xbe, 0xfb, 0xef, 0xeb,
    0xee, 0xfe, 0xfb, 0xef,
    0xef, 0xbe, 0xef, 0xbb,
    0xee, 0xfb, 0xef, 0xef,
    0xbe, 0xfe, 0xfe, 0xef,
    0xef, 0xef, 0xbe, 0xfe,
    0xef, 0xef, 0xef, 0xfe,
    0xbb, 0xef, 0xef, 0xbb,

    // -----

    // stairs down top-left
    0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff,
    0x55, 0x55, 0x55, 0x55,
    0x55, 0x55, 0x55, 0x55,
    0x55, 0x55, 0x55, 0x55,
    0x55, 0x55, 0x55, 0x55,
    0x55, 0x55, 0x55, 0x55,
    0x55, 0x55, 0x55, 0x11,

    // stairs down top-right
    0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff,
    0x55, 0x55, 0x55, 0x55,
    0x55, 0x55, 0x11, 0x15,
    0x55, 0x55, 0x11, 0x15,
    0x51, 0x12, 0x11, 0x15,
    0x51, 0x12, 0x11, 0x15,
    0x21, 0x12, 0x11, 0x15,

    // stairs down bottom-left
    0x55, 0x55, 0x55, 0x11,
    0x55, 0x51, 0x12, 0x11,
    0x55, 0x51, 0x12, 0x11,
    0x11, 0x21, 0x12, 0x11,
    0x55, 0x55, 0x55, 0x55,
    0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff,

    // stairs down bottom-right
    0x21, 0x12, 0x11, 0x15,
    0x21, 0x12, 0x11, 0x15,
    0x21, 0x12, 0x11, 0x15,
    0x21, 0x12, 0x11, 0x15,
    0x55, 0x55, 0x55, 0x55,
    0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff,

    // -----

    // stairs up top-left
    0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xf1,
    0xff, 0xff, 0xff, 0xf1,
    0xff, 0xff, 0x11, 0x21,
    0xff, 0xff, 0x11, 0x21,

    // stairs up top-right
    0xff, 0xff, 0xf1, 0x11,
    0xff, 0xff, 0xf1, 0x11,
    0xff, 0x11, 0x21, 0x11,
    0xff, 0x11, 0x21, 0x11,
    0x12, 0x11, 0x21, 0x11,
    0x12, 0x11, 0x21, 0x11,
    0x12, 0x11, 0x21, 0x11,
    0x12, 0x11, 0x25, 0x55,

    // stairs up bottom-left
    0xf1, 0x12, 0x11, 0x21,
    0xf1, 0x12, 0x11, 0x21,
    0xf1, 0x12, 0x11, 0x21,
    0xf1, 0x12, 0x11, 0x25,
    0xf1, 0x12, 0x11, 0x25,
    0xf1, 0x12, 0x55, 0x55,
    0xf1, 0x12, 0x55, 0x55,
    0xff, 0xff, 0xff, 0xff,

    // stairs up bottom-right
    0x12, 0x11, 0x25, 0x55,
    0x12, 0x55, 0x55, 0x55,
    0x12, 0x55, 0x55, 0x55,
    0x55, 0x55, 0x55, 0x55,
    0x55, 0x55, 0x55, 0x55,
    0x55, 0x55, 0x55, 0x55,
    0x55, 0x55, 0x55, 0x55,
    0xff, 0xff, 0xff, 0xff,

    // -----

    // cloud 0
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,

    // cloud 1
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x14, 0x44, 0x11,
    0x11, 0x41, 0x11, 0x11,
    0x11, 0x41, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,

    // cloud 2
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x14, 0x41,
    0x11, 0x14, 0x44, 0x11,
    0x11, 0x44, 0x11, 0x11,
    0x11, 0x41, 0x11, 0x11,
    0x11, 0x41, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,

    // -----

    // ore/cloud cactus top-left
    0xff, 0xff, 0xff, 0xff,
    0xf5, 0x5f, 0xff, 0xff,
    0xf5, 0x35, 0xff, 0xf5,
    0xf5, 0x43, 0x5f, 0xf5,
    0xf5, 0x44, 0x5f, 0x53,
    0xff, 0x54, 0x35, 0x53,
    0xff, 0x54, 0x45, 0x33,
    0xff, 0x54, 0x45, 0x33,

    // ore/cloud cactus top-right
    0xff, 0xff, 0xff, 0xff,
    0xff, 0x5f, 0xff, 0xff,
    0xf5, 0x35, 0xff, 0x55,
    0x53, 0x45, 0xf5, 0x35,
    0x54, 0x45, 0x53, 0x45,
    0x54, 0x45, 0x34, 0x5f,
    0x54, 0x53, 0x44, 0x5f,
    0x55, 0x34, 0x45, 0xff,

    // ore/cloud cactus bottom-left
    0xff, 0xf5, 0x45, 0x33,
    0xf5, 0x55, 0x55, 0x33,
    0x53, 0x33, 0x53, 0x34,
    0x55, 0x43, 0x53, 0x34,
    0xf5, 0x55, 0x33, 0x34,
    0xff, 0xf5, 0x33, 0x34,
    0xff, 0xff, 0x53, 0x44,
    0xff, 0xff, 0xf5, 0x55,

    // ore/cloud cactus bottom-right
    0x53, 0x34, 0x45, 0xff,
    0x45, 0x44, 0x45, 0x55,
    0x45, 0x55, 0x53, 0x35,
    0x45, 0x33, 0x34, 0x5f,
    0x45, 0x34, 0x45, 0xff,
    0x45, 0x55, 0x5f, 0xff,
    0x45, 0xff, 0xff, 0xff,
    0x5f, 0xff, 0xff, 0xff,

    // ---

    // sapling

    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x21, 0x11, 0x11,
    0x11, 0x11, 0x12, 0x11,
    0x12, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x16,
    0x11, 0x11, 0x11, 0x6a,

    0x22, 0xff, 0xff, 0x22,
    0x11, 0x22, 0x22, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x16,
    0x11, 0x11, 0x11, 0x6a,

    0x21, 0x11, 0x11, 0x11,
    0x21, 0x11, 0x11, 0x11,
    0xf2, 0x11, 0x11, 0x11,
    0xf2, 0x11, 0x11, 0x11,
    0xf2, 0x11, 0x11, 0x11,
    0xf2, 0x11, 0x11, 0x11,
    0x21, 0x11, 0x11, 0x16,
    0x21, 0x11, 0x11, 0x6a,

    0xff, 0xff, 0xff, 0x22,
    0xff, 0xf2, 0x22, 0x11,
    0xff, 0x21, 0x11, 0x11,
    0xf2, 0x11, 0x11, 0x11,
    0xf2, 0x11, 0x11, 0x11,
    0xf2, 0x11, 0x11, 0x11,
    0x21, 0x11, 0x11, 0x16,
    0x21, 0x11, 0x11, 0x6a,

    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x12, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x61, 0x11, 0x12, 0x11,
    0xb6, 0x11, 0x11, 0x11,
    0x61, 0x11, 0x11, 0x11,

    0x22, 0xff, 0xff, 0x22,
    0x11, 0x22, 0x22, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x61, 0x11, 0x11, 0x11,
    0xb6, 0x11, 0x11, 0x11,
    0x61, 0x11, 0x11, 0x11,

    0x11, 0x11, 0x11, 0x13,
    0x11, 0x11, 0x11, 0x13,
    0x11, 0x11, 0x11, 0x3f,
    0x11, 0x11, 0x11, 0x3f,
    0x11, 0x11, 0x11, 0x3f,
    0x61, 0x11, 0x11, 0x3f,
    0xb6, 0x11, 0x11, 0x13,
    0x61, 0x11, 0x11, 0x13,

    0x22, 0xff, 0xff, 0xff,
    0x11, 0x22, 0x2f, 0xff,
    0x11, 0x11, 0x11, 0xff,
    0x11, 0x11, 0x11, 0x3f,
    0x11, 0x11, 0x11, 0x3f,
    0x61, 0x11, 0x11, 0x3f,
    0xb6, 0x11, 0x11, 0x13,
    0x61, 0x11, 0x11, 0x13,

    0x11, 0x11, 0x11, 0x6a,
    0x11, 0x11, 0x11, 0x66,
    0x11, 0x21, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x21,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x12, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,

    0x11, 0x11, 0x11, 0x6a,
    0x11, 0x11, 0x11, 0x66,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x33, 0x33, 0x11,
    0x33, 0xff, 0xff, 0x33,

    0x21, 0x11, 0x11, 0x6a,
    0x21, 0x11, 0x11, 0x66,
    0xf2, 0x11, 0x11, 0x11,
    0xf2, 0x11, 0x11, 0x11,
    0xf2, 0x11, 0x11, 0x11,
    0xf2, 0x11, 0x11, 0x11,
    0x21, 0x11, 0x11, 0x11,
    0x21, 0x11, 0x11, 0x11,

    0x21, 0x11, 0x11, 0x6a,
    0x21, 0x11, 0x11, 0x66,
    0xf2, 0x11, 0x11, 0x11,
    0xf2, 0x11, 0x11, 0x11,
    0xf2, 0x11, 0x11, 0x11,
    0xff, 0x11, 0x11, 0x11,
    0xff, 0xf3, 0x33, 0x11,
    0xff, 0xff, 0xff, 0x33,

    0xa6, 0x11, 0x11, 0x11,
    0x66, 0x11, 0x12, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x21, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,

    0xa6, 0x11, 0x11, 0x11,
    0x66, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x33, 0x33, 0x11,
    0x33, 0xff, 0xff, 0x33,

    0xa6, 0x11, 0x11, 0x13,
    0x66, 0x11, 0x11, 0x13,
    0x11, 0x11, 0x11, 0x3f,
    0x11, 0x11, 0x11, 0x3f,
    0x11, 0x11, 0x11, 0x3f,
    0x11, 0x11, 0x11, 0x3f,
    0x11, 0x11, 0x11, 0x13,
    0x11, 0x11, 0x11, 0x13,

    0xa6, 0x11, 0x11, 0x13,
    0x66, 0x11, 0x11, 0x13,
    0x11, 0x11, 0x11, 0x3f,
    0x11, 0x11, 0x11, 0x3f,
    0x11, 0x11, 0x11, 0x3f,
    0x11, 0x11, 0x13, 0xff,
    0x11, 0x33, 0x3f, 0xff,
    0x33, 0xff, 0xff, 0xff,

    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x12, 0x21, 0x11,
    0x11, 0x21, 0x14, 0x11,
    0x11, 0x21, 0x14, 0x11,
    0x11, 0x14, 0x41, 0x11,
    0x11, 0x11, 0x11, 0x16,
    0x11, 0x11, 0x11, 0x6a,

    0xa6, 0x11, 0x11, 0x11,
    0x66, 0x11, 0x11, 0x11,
    0x11, 0x12, 0x21, 0x11,
    0x11, 0x21, 0x14, 0x11,
    0x11, 0x21, 0x14, 0x11,
    0x11, 0x14, 0x41, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
};

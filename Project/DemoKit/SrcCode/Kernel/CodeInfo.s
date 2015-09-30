/*;   Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
 *;
 *;   @file       CodeInfo.s
 *;   @ingroup
 *;
 *;   @brief      Code section information
 *;               Code section information
 *;
 *;   @version    V1.20.000
 *;   @author     Novatek
 *;   @date       2012/07/19
 */

/*
Code Info: (*.s) 0x2F0 ~ 0x400 = 272 bytes = 68 WORDS

CODE partition(8x32) ------ for BOOT-FLOW load other parts, for LD load part-1
                            for BOOT-FLOW clear ZI
//[0x300]
    //ZI base ------- this value will be generated by linker
//[0x304]
    //ZI limit ------- this value will be generated by linker
//[0x308]
    //part-1 addr ------- this value will be generated by linker
//[0x30c]
    //part-1 size ------- this value will be generated by linker
*/

/* NOTE: 1 zi section + max 31 section */

 .global            _image_general_zi_zi_base
 .global            _image_general_zi_zi_limit
 .global            _section_01_addr
 .global            _section_01_size
 .global            _section_02_addr
 .global            _section_02_size
 .global            _section_03_addr
 .global            _section_03_size
 .global            _section_04_addr
 .global            _section_04_size
 .global            _section_05_addr
 .global            _section_05_size
 .global            _section_06_addr
 .global            _section_06_size
 .global            _section_07_addr
 .global            _section_07_size
 .global            _section_08_addr
 .global            _section_08_size
 .global            _section_09_addr
 .global            _section_09_size
 .global            _section_10_addr
 .global            _section_10_size

 .data
 .org  0x00 /* CODEINFO SIGNATURE OFFSET, DO NOT MODIFY! */
 .ascii             "CODEINFO        "
 .org  0x10 /* ZI_SECTION_OFFSET, DO NOT MODIFY! */
 .word              _image_general_zi_zi_base
 .word              _image_general_zi_zi_limit
 .org  0x18 /* CODE_SECTION_OFFSET, DO NOT MODIFY! */
 .word              _section_01_addr
 .word              _section_01_size
 .word              _section_02_addr
 .word              _section_02_size
 .word              _section_03_addr
 .word              _section_03_size
 .word              _section_04_addr
 .word              _section_04_size
 .word              _section_05_addr
 .word              _section_05_size
 .word              _section_06_addr
 .word              _section_06_size
 .word              _section_07_addr
 .word              _section_07_size
 .word              _section_08_addr
 .word              _section_08_size
 .word              _section_09_addr
 .word              _section_09_size
 .word              _section_10_addr
 .word              _section_10_size
 .word              0xffffffff /* MARK: END OF CODE_SECTION */


typedef u32(*copy_sd_mmc_to_mem)
(u32 channel, u32 start_block, u16 block_size, u32 *trg, u32 init);

int copy_sd_mmc_to_ddr(u32 start_block, u16 block_cnt, u32 *trg)
{
    unsigned long ch = *(volatile u32 *)(0xD0037488);
    copy_sd_mmc_to_mem do_copy = (copy_sd_mmc_to_mem) (*(u32 *) (0xD0037F98));

	if (ch == 0xEB000000) {
        do_copy(0, start_block, block_cnt, trg, 0);
    } else if (ch == 0xEB200000) {
        do_copy(2, start_block, block_cnt, trg, 0);
    } 

    return 0;
}

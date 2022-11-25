#include "page.h"

class Memory{
public:
    PhysicalMemory *pmm;
    Memory(uint64_t max_range){
        pmm = new PhysicalMemory(max_range);
    }
    ~Memory(){
        delete pmm;
    }
    int createRootPageTable(){
        uint64_t root = pmm->findUsable(SV39::PageSize);
        if(!root)
            return -1;
        return pmm->insertBlock(Block(root, SV39::PageSize, true));
    }
    uint64_t allocateMemory(uint64_t root, uint64_t v_addr, uint64_t length){
        length = (length + SV39::PageSize - 1) / SV39::PageSize * SV39::PageSize;
        uint64_t p_addr = pmm->findUsable(length);
        if(p_addr == 0ull)
            return 0ull;
        if(pmm->insertBlock(Block(p_addr, length, true))) {
            return 0ull;
        }
        uint64_t pt_idx[3] = {~0ull, ~0ull, ~0ull};// root, I, II
        for(uint64_t iter = 0ull; iter < length; iter += SV39::PageSize){
            if(SV39::VAextract((v_addr + iter), 0) != pt_idx[0]){ //may need a new entry in root PageTable
                pt_idx[0] = SV39::VAextract((v_addr + iter), 0);
                if( (pmm->readWord<uint64_t>(root + pt_idx[0]) & SV39::V) == 0ull ){ // entry not exist
                    auto pt_addr = pmm->findUsable(SV39::PageSize);
                    if(!pt_addr){
                        return 0;
                    }
                    pmm->insertBlock(Block(pt_addr, SV39::PageSize, true)); // create and insert a PT1
                    //pmm->writeWord
                }
            }
        }
        return root;
    }
};

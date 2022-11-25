#include "page.h"
#include <iostream>
#include <list>

int main(){
    Block* b;
    b = new Block{0x1000ull, 4096*2, true};
    b->pages->data[0] = '\x78';
    b->pages->data[1] = '\x56';
    b->pages->data[2] = '\x34';
    b->pages->data[3] = '\x12';
    PhysicalMemory p{0x10000ull};
    auto x = p.insertBlock(std::move(*b));
    delete b;
    p.insertBlock(Block{0x5000ull, 4096, true});
    for(auto &it: *p.blocks){
        printf("0x%16lx - 0x%16lx : 0x%016lx\n", it.addr, it.end(), (uint64_t)it.pages);
        //std::cout << it.addr << " " << it.size << " +" << it.pages << std::endl;
    }
    printf("0x%16lx\n", p.findUsable(4096 *4));
    printf("0x%08x\n", p.readWord<uint32_t>(0x1000ull));
    return 0;
}
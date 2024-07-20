int mem [1000];

int mem_read(int addr) {
    return mem[addr];
}

void mem_write(int addr, int val) {
    mem[addr] = val;
}

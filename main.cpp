#include <iostream>
#include <sys/types.h>
#include <sys/sysctl.h>
#include <mach/mach.h>
#include <mach/vm_statistics.h>
#include <mach/mach_types.h>
#include <mach/mach_init.h>
#include <mach/mach_host.h>
#include <unistd.h> 

void clearConsole() {
    std::cout << "\033[2J\033[H"; 
}

void getCPUUsage() {
    host_cpu_load_info_data_t cpuinfo;
    mach_msg_type_number_t count = HOST_CPU_LOAD_INFO_COUNT;
    kern_return_t kr = host_statistics(mach_host_self(), HOST_CPU_LOAD_INFO, (host_info_t)&cpuinfo, &count);
    
    if (kr != KERN_SUCCESS) {
        std::cerr << "Ошибка при получении информации о CPU" << std::endl;
        return;
    }

    unsigned long long totalTicks = 0;
    for (int i = 0; i < CPU_STATE_MAX; i++) {
        totalTicks += cpuinfo.cpu_ticks[i];
    }

    unsigned long long idleTicks = cpuinfo.cpu_ticks[CPU_STATE_IDLE];
    unsigned long long usedTicks = totalTicks - idleTicks;
    double usage = (double)usedTicks / totalTicks * 100.0;
    std::cout << "Использование CPU: " << usage << "%" << std::endl;
}

void getMemoryUsage() {
    vm_size_t page_size;
    mach_port_t mach_port;
    mach_msg_type_number_t count;
    vm_statistics64_data_t vm_stats;

    mach_port = mach_host_self();
    count = sizeof(vm_stats) / sizeof(natural_t);
    
    if (KERN_SUCCESS == host_page_size(mach_port, &page_size) &&
        KERN_SUCCESS == host_statistics64(mach_port, HOST_VM_INFO,
                                        (host_info64_t)&vm_stats, &count)) {
        
        long long free_memory = (int64_t)vm_stats.free_count * (int64_t)page_size;
        long long used_memory = ((int64_t)vm_stats.active_count +
                                 (int64_t)vm_stats.inactive_count +
                                 (int64_t)vm_stats.wire_count) *  (int64_t)page_size;
        
                                 std::cout << "Использование памяти: " << used_memory / 1024 / 1024 << " MB" << std::endl;
        std::cout << "Свободная память: " << free_memory / 1024 / 1024 << " MB" << std::endl;
    } 
    else {
        std::cerr << "Ошибка при получении информации о памяти" << std::endl;
    }
}

int main() {
    while (true) {
        clearConsole(); 
        getCPUUsage();
        getMemoryUsage();
        sleep(1); 
    }
    return 0;
}
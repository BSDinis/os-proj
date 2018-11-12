SO Projeto 2 - Grupo 69 - Baltasar Dinis

README generated at: Mon Nov 12 03:26:05 WET 2018

STRUCTURE
  (relative to project root, where README.txt is)

Makefile: global makefile

CircuitRouter-ParSolver: solution of the project
  Contains: Makefile, src files

CircuitRouter-SeqSolver: sequential router solver
  Contains: Makefile, src files

results: speedup data from tests
  Contains: *.speedups.csv files

lib: library of utilities (provided)
  Contains: Makefile, src files

bin: script folder
  Contains: doTest.sh  


HOW TO COMPILE
Easy: make

HOW TO RUN (after compiling)

Via doTest.sh: run ./bin/doTest.sh <max number of threads> <input file>
Via binary:    run ./CircuitRouter-ParSolver/CircuitRouter-ParSolver -t <number of threads> <input file>

OS information:
Linux althea 4.18.16-200.fc28.x86_64 #1 SMP Sat Oct 20 23:53:47 UTC 2018 x86_64 x86_64 x86_64 GNU/Linux

CPU information:
Via cat /proc/cpuinfo:
processor : 0 vendor_id : GenuineIntel cpu family : 6 model : 78 model name : Intel(R) Core(TM) i5-6200U CPU @ 2.30GHz stepping : 3 microcode : 0xc6 cpu MHz : 2610.879 cache size : 3072 KB physical id : 0 siblings : 4 core id : 0 cpu cores : 2 apicid : 0 initial apicid : 0 fpu : yes fpu_exception : yes cpuid level : 22 wp : yes flags : fpu vme de pse tsc msr pae mce cx8 apic sep mtrr pge mca cmov pat pse36 clflush dts acpi mmx fxsr sse sse2 ss ht tm pbe syscall nx pdpe1gb rdtscp lm constant_tsc art arch_perfmon pebs bts rep_good nopl xtopology nonstop_tsc cpuid aperfmperf tsc_known_freq pni pclmulqdq dtes64 monitor ds_cpl vmx est tm2 ssse3 sdbg fma cx16 xtpr pdcm pcid sse4_1 sse4_2 x2apic movbe popcnt tsc_deadline_timer aes xsave avx f16c rdrand lahf_lm abm 3dnowprefetch cpuid_fault epb invpcid_single pti ssbd ibrs ibpb stibp tpr_shadow vnmi flexpriority ept vpid fsgsbase tsc_adjust bmi1 avx2 smep bmi2 erms invpcid mpx rdseed adx smap clflushopt intel_pt xsaveopt xsavec xgetbv1 xsaves dtherm ida arat pln pts hwp hwp_notify hwp_act_window hwp_epp flush_l1d bugs : cpu_meltdown spectre_v1 spectre_v2 spec_store_bypass l1tf bogomips : 4800.00 clflush size : 64 cache_alignment : 64 address sizes : 39 bits physical, 48 bits virtual power management: processor : 1 vendor_id : GenuineIntel cpu family : 6 model : 78 model name : Intel(R) Core(TM) i5-6200U CPU @ 2.30GHz stepping : 3 microcode : 0xc6 cpu MHz : 2630.725 cache size : 3072 KB physical id : 0 siblings : 4 core id : 1 cpu cores : 2 apicid : 2 initial apicid : 2 fpu : yes fpu_exception : yes cpuid level : 22 wp : yes flags : fpu vme de pse tsc msr pae mce cx8 apic sep mtrr pge mca cmov pat pse36 clflush dts acpi mmx fxsr sse sse2 ss ht tm pbe syscall nx pdpe1gb rdtscp lm constant_tsc art arch_perfmon pebs bts rep_good nopl xtopology nonstop_tsc cpuid aperfmperf tsc_known_freq pni pclmulqdq dtes64 monitor ds_cpl vmx est tm2 ssse3 sdbg fma cx16 xtpr pdcm pcid sse4_1 sse4_2 x2apic movbe popcnt tsc_deadline_timer aes xsave avx f16c rdrand lahf_lm abm 3dnowprefetch cpuid_fault epb invpcid_single pti ssbd ibrs ibpb stibp tpr_shadow vnmi flexpriority ept vpid fsgsbase tsc_adjust bmi1 avx2 smep bmi2 erms invpcid mpx rdseed adx smap clflushopt intel_pt xsaveopt xsavec xgetbv1 xsaves dtherm ida arat pln pts hwp hwp_notify hwp_act_window hwp_epp flush_l1d bugs : cpu_meltdown spectre_v1 spectre_v2 spec_store_bypass l1tf bogomips : 4800.00 clflush size : 64 cache_alignment : 64 address sizes : 39 bits physical, 48 bits virtual power management: processor : 2 vendor_id : GenuineIntel cpu family : 6 model : 78 model name : Intel(R) Core(TM) i5-6200U CPU @ 2.30GHz stepping : 3 microcode : 0xc6 cpu MHz : 2579.371 cache size : 3072 KB physical id : 0 siblings : 4 core id : 0 cpu cores : 2 apicid : 1 initial apicid : 1 fpu : yes fpu_exception : yes cpuid level : 22 wp : yes flags : fpu vme de pse tsc msr pae mce cx8 apic sep mtrr pge mca cmov pat pse36 clflush dts acpi mmx fxsr sse sse2 ss ht tm pbe syscall nx pdpe1gb rdtscp lm constant_tsc art arch_perfmon pebs bts rep_good nopl xtopology nonstop_tsc cpuid aperfmperf tsc_known_freq pni pclmulqdq dtes64 monitor ds_cpl vmx est tm2 ssse3 sdbg fma cx16 xtpr pdcm pcid sse4_1 sse4_2 x2apic movbe popcnt tsc_deadline_timer aes xsave avx f16c rdrand lahf_lm abm 3dnowprefetch cpuid_fault epb invpcid_single pti ssbd ibrs ibpb stibp tpr_shadow vnmi flexpriority ept vpid fsgsbase tsc_adjust bmi1 avx2 smep bmi2 erms invpcid mpx rdseed adx smap clflushopt intel_pt xsaveopt xsavec xgetbv1 xsaves dtherm ida arat pln pts hwp hwp_notify hwp_act_window hwp_epp flush_l1d bugs : cpu_meltdown spectre_v1 spectre_v2 spec_store_bypass l1tf bogomips : 4800.00 clflush size : 64 cache_alignment : 64 address sizes : 39 bits physical, 48 bits virtual power management: processor : 3 vendor_id : GenuineIntel cpu family : 6 model : 78 model name : Intel(R) Core(TM) i5-6200U CPU @ 2.30GHz stepping : 3 microcode : 0xc6 cpu MHz : 2463.398 cache size : 3072 KB physical id : 0 siblings : 4 core id : 1 cpu cores : 2 apicid : 3 initial apicid : 3 fpu : yes fpu_exception : yes cpuid level : 22 wp : yes flags : fpu vme de pse tsc msr pae mce cx8 apic sep mtrr pge mca cmov pat pse36 clflush dts acpi mmx fxsr sse sse2 ss ht tm pbe syscall nx pdpe1gb rdtscp lm constant_tsc art arch_perfmon pebs bts rep_good nopl xtopology nonstop_tsc cpuid aperfmperf tsc_known_freq pni pclmulqdq dtes64 monitor ds_cpl vmx est tm2 ssse3 sdbg fma cx16 xtpr pdcm pcid sse4_1 sse4_2 x2apic movbe popcnt tsc_deadline_timer aes xsave avx f16c rdrand lahf_lm abm 3dnowprefetch cpuid_fault epb invpcid_single pti ssbd ibrs ibpb stibp tpr_shadow vnmi flexpriority ept vpid fsgsbase tsc_adjust bmi1 avx2 smep bmi2 erms invpcid mpx rdseed adx smap clflushopt intel_pt xsaveopt xsavec xgetbv1 xsaves dtherm ida arat pln pts hwp hwp_notify hwp_act_window hwp_epp flush_l1d bugs : cpu_meltdown spectre_v1 spectre_v2 spec_store_bypass l1tf bogomips : 4800.00 clflush size : 64 cache_alignment : 64 address sizes : 39 bits physical, 48 bits virtual power management:

Via lscpu:
Architecture: x86_64 CPU op-mode(s): 32-bit, 64-bit Byte Order: Little Endian CPU(s): 4 On-line CPU(s) list: 0-3 Thread(s) per core: 2 Core(s) per socket: 2 Socket(s): 1 NUMA node(s): 1 Vendor ID: GenuineIntel CPU family: 6 Model: 78 Model name: Intel(R) Core(TM) i5-6200U CPU @ 2.30GHz Stepping: 3 CPU MHz: 2610.879 CPU max MHz: 2800.0000 CPU min MHz: 400.0000 BogoMIPS: 4800.00 Virtualization: VT-x L1d cache: 32K L1i cache: 32K L2 cache: 256K L3 cache: 3072K NUMA node0 CPU(s): 0-3 Flags: fpu vme de pse tsc msr pae mce cx8 apic sep mtrr pge mca cmov pat pse36 clflush dts acpi mmx fxsr sse sse2 ss ht tm pbe syscall nx pdpe1gb rdtscp lm constant_tsc art arch_perfmon pebs bts rep_good nopl xtopology nonstop_tsc cpuid aperfmperf tsc_known_freq pni pclmulqdq dtes64 monitor ds_cpl vmx est tm2 ssse3 sdbg fma cx16 xtpr pdcm pcid sse4_1 sse4_2 x2apic movbe popcnt tsc_deadline_timer aes xsave avx f16c rdrand lahf_lm abm 3dnowprefetch cpuid_fault epb invpcid_single pti ssbd ibrs ibpb stibp tpr_shadow vnmi flexpriority ept vpid fsgsbase tsc_adjust bmi1 avx2 smep bmi2 erms invpcid mpx rdseed adx smap clflushopt intel_pt xsaveopt xsavec xgetbv1 xsaves dtherm ida arat pln pts hwp hwp_notify hwp_act_window hwp_epp flush_l1d

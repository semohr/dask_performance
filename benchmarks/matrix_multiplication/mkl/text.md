mpiexec.hydra(1)                                                                                 MPI                                                                                 mpiexec.hydra(1)



NAME
       mpiexec.hydra -  Run an MPI program

SYNOPSIS
       mpiexec.hydra <g-options> <l-options> <executable> [ : <l-options> <executable>  ]

       where g-options are global options that apply to all MPI processes, l-options are local options that apply to single argument set, executable is the name of the executable file


GLOBAL OPTIONS
       -hostfile <hostfile> or -f <hostfile>
              Use this option to specify host names on which to run the application.  If a host name is repeated, this name is used only once.

       -machinefile <machine file> or -machine <machine file>
              Use this option to control process placement through a machine file.

       -genv <ENVVAR> <value>
              Use this option to set the ENVVAR environment variable to the specified value for all MPI processes.

       -genvall
              Use this option to enable propagation of all environment variables to all MPI processes.

       -genvnone
              Use this option to suppress propagation of any environment variables to any MPI processes.

       -genvlist <list>
              Use this option to pass a list of environment variables with their current values.  list is a comma separated list of environment variables to be sent to all MPI processes.

       -pmi-connect <mode>
              Use this option to choose the caching mode of process management interface (PMI) message.

       -perhost <# of processes>, -ppn <# of processes, -grr <# of processes>
              Use this option to place the specified number of consecutive MPI processes on every host in the group using round robin scheduling.

       -rr    Use this option to place consecutive MPI processes on different hosts using the round robin scheduling. This option is equivalent to -perhost 1

       -trace <profiling library> or -t <profiling library>
              Use  this  option  to  profile your MPI application with Intel(R) Trace Collector using the indicated profiling_library If you do not specify profiling_library , the default profiling
              library libVT.so is used.

       -trace-imbalance
              Use this option to profile your MPI application with Intel Trace Collector using the libVTim.so library.

       -mps   Use this option to collect statistics from your MPI application using MPI Performance Snapshot, which can collect hardware performance metrics, memory consumption data,  internal  MPI
              imbalance  and  OpenMP*  imbalance  statistics. When you use this option, a new folder stat_<date>-<time> is generated that contains the statistics. You can analyze the collected data
              with the mps utility, which provides a high-level overview of your application performance.  MPI Performance Snapshot is available as part of Intel(R) Trace Analyzer and Collector.

       -check-mpi [<checking_library>]
              Use this option to check your MPI application for correctness using the specified checking_library If you do not specify checking_library , the default checking library libVTmc.so  is
              used.

       -trace-pt2pt
              Use this option to collect the information about point-to-point operations using Intel Trace Analyzer and Collector. The option requires that you also use the -trace option.

       -trace-collectives
              Use this option to collect the information about collective operations using Intel Trace Analyzer and Collector. The option requires that you also use the -trace option.

       -configfile <filename>
              Use this option to specify the file filename that contains the command-line options.

       -branch-count
              Use this option to restrict the number of child management processes launched by the Hydra process manager, or by each pmi_proxy management process.

       -pmi-aggregate or -pmi-noaggregate
              Use this option to switch on or off, respectively, the aggregation of the PMI requests. The default value is -pmi-aggregate , which means the aggregation is enabled by default.

       -tv    Use this option to run executable under the TotalView* debugger.

       -tva <pid>
              Use this option to attach the TotalView* debugger to an existing MPI job. Use the mpiexec.hydra process id as pid

       -gdb   Use this option to run an executable under the GNU* debugger.

       -gdba <pid>
              Use this option to attach the GNU* debugger to the existing MPI job.

       -nolocal
              Use  this  option to avoid running the executable on the host where mpiexec.hydra is launched. You can use this option on clusters that deploy a dedicated master node for starting the
              MPI jobs and a set of dedicated compute nodes for running the actual MPI processes.

       -hosts <nodelist>
              Use this option to specify a particular nodelist on which the MPI processes should be run.

       -iface <interface>
              Use this option to choose the appropriate network interface.

       -demux <mode>
              Use this option to set the polling mode for multiple I/O. The default value is poll

       -enable-x or -disable-x
              Use this option to control the Xlib* traffic forwarding. The default value is -disable-x , which means the Xlib traffic is not forwarded.

       -l or -prepend-rank
              Use this option to insert the MPI process rank at the beginning of all lines written to the standard output.

       -tune [<arg>]
              Use this option to optimize the Intel MPI Library performance by using the data collected by the mpitune utility.

       -ilp64 Use this option to preload the ILP64 interface.

       -s <spec>
              Use this option to direct standard input to the specified MPI processes.

       -noconf
              Use this option to disable processing of the mpiexec.hydra configuration files.

       -ordered-output
              Use this option to avoid intermingling of data output from the MPI processes. This option affects both the standard output and the standard error streams.

       -path <directory>
              Use this option to specify the path to the executable file.

       -cleanup
              Use this option to create a temporary file containing information about the launched processes.

       -tmpdir <dir>
              Use this option to set a directory for temporary files.

       -version or -V
              Use this option to display the version of the Intel(R) MPI Library.

       -info  Use this option to display build information of the Intel MPI Library. When this option is used, the other command line arguments are ignored.

       -use-app-topology <value>
              Use this option to optimally place MPI processes on the cluster.

       -localhost
              Use this option to explicitly specify the local host name for the launching node.

       -rmk <RMK>
              Use this option to select a resource management kernel to be used. Intel MPI Library only supports pbs

       -outfile-pattern <file>
              Use this option to redirect stdout to the specified file.

       -errfile-pattern <file>
              Use this option to redirect stderr to the specified file.

       -gpath <path>
              Use this option to specify the path to the executable file.

       -gwdir <dir>
              Use this option to specify the working directory in which the executable file runs.

       -gumask <umask>
              Use this option to perform the "umask <umask>" command for the remote executable file.

       -gdb-ia
              Use this option to run processes under Intel(R) architecture specific GNU* debugger.

       -prepend-pattern <pattern>
              Use this option to specify the pattern that is prepended to the process output.

       -verbose or -v
              Use this option to print debug information from mpiexec.hydra

       -print-rank-map
              Use this option to print out the MPI rank mapping.

       -print-all-exitcodes
              Use this option to print the exit codes of all processes.

       -bootstrap <bootstrap server>
              Use this option to select a built-in bootstrap server to use. A bootstrap server is the basic remote node access mechanism that is provided by the system. Hydra supports multiple run‐
              time bootstrap servers such as ssh, rsh, pdsh, fork, persist, slurm, ll, lsf, or sge to launch MPI processes. The default bootstrap server is ssh.

       -bootstrap-exec <bootstrap server>
              Use this option to set the executable to be used as a bootstrap server.

       -bootstrap-exec-args <args>
              Use this option to provide the additional parameters to the bootstrap server executable file.

       -prefork
              Use this option to enable a new method for application processes startup. The new processes start up method allows you to reduce the number of file system operations during the appli‐
              cation startup phase, which reduces the process loading time.

       -binding <options>
              Use this option to pin or bind MPI processes to a particular processor and avoid undesired process migration.

       -gtool <options>
              Use this option to launch such tools as Intel(R) VTune(TM) Amplifier XE, Intel(R) Advisor, Valgrind*, and GNU* Debugger (GDB*) for the specified processes  through  the  mpiexec.hydra
              and mpirun commands.

       -gtoolfile <gtool_config_file>
              Use this option to specify the -gtool parameters in a configuration file.


LOCAL OPTIONS
       -n <# of processes> or -np <# of processes>
              Use this option to set the number of MPI processes to run with the current argument set.

       -env <ENVVAR> <value>
              Use this option to set the ENVVAR environment variable to the specified value for all MPI processes in the current argument set.

       -envall
              Use this option to propagate all environment variables in the current argument set.

       -envnone
              Use this option to suppress propagation of any environment variables to the MPI processes in the current argument set.

       -envlist <list>
              Use this option to pass a list of environment variables with their current values.

       -host <nodename>
              Use this option to specify a particular nodename on which the MPI processes are to be run.

       -path <directory>
              Use this option to specify the path to the executable file to be run in the current argument set.

       -wdir <directory>
              Use this option to specify the working directory in which the executable file runs in the current argument set.

       -umask <umask>
              Use this option to perform the umask <umask> command for the remote executable file

       -hostos <host OS>
              Use  this  option to specify an operating system installed on a particular host. MPI processes are launched on each host in accordance with this option specified. The default value is
              linux


EXTENDED FABRIC CONTROL OPTIONS
       -rdma  Use this option to select an RDMA-capable network fabric. The application attempts to use the first available RDMA-capable network fabric from the list: dapl,ofa. If no such fabric is
              available, another fabric from the list tcp,tmi,ofi is used.

       -RDMA  Use  this  option  to  select  an RDMA-capable network fabric. The application attempts to use the first available RDMA-capable network fabric from the list: dapl,ofa. The application
              fails if no such fabric is found.

       -dapl  Use this option to select a DAPL-capable network fabric. The application attempts to use a DAPL-capable network fabric. If no such fabric is available, another fabric  from  the  list
              tcp,tmi,ofa,ofi is used.

       -DAPL  Use this option to select a DAPL-capable network fabric. The application fails if no such fabric is found.

       -ib    Use  this  option  to select an OFA-capable network fabric. The application attempts to use an OFA-capable network fabric. If no such fabric is available, another fabric from the list
              dapl,tcp,tmi,ofi is used.

       -IB    Use this option to select an OFA-capable network fabric. The application fails if no such fabric is found.

       -tmi   Use this option to select a TMI-capable network fabric. The application attempts to use a TMI-capable network fabric. If no such fabric is available,  another  fabric  from  the  list
              dapl,tcp,ofa,ofi is used.

       -TMI   Use this option to select a TMI-capable network fabric. The application fails if no such fabric is found.

       -mx    Use  this option to select the Myrinet MX* network fabric. The application attempts to use the Myrinet MX* network fabric. If no such fabric is available, another fabric from the list
              dapl,tcp,ofa,ofi is used.

       -MX    Use this option to select Myrinet MX* network fabric. The application fails if no such fabric is found.

       -psm   Use this option to select a PSM-capable network fabric: Intel(R) True Scale Fabric or Intel(R) Omni-Path Fabric in PSM-compatibility mode. The application attempts to use a  PSM-capa‐
              ble network fabric. If no such fabric is available, another fabric from the list dapl,tcp,ofa,ofi is used.

       -PSM   Use  this  option to select a PSM-capable network fabric: Intel(R) True Scale Fabric or Intel(R) Omni-Path Fabric in PSM-compatibility mode. The application fails if no such fabric is
              found.

       -psm2  Use this option to select the Intel Omni-Path Fabric. The application attempts to use the Intel Omni-Path Fabric. If no  such  fabric  is  available,  another  fabric  from  the  list
              dapl,tcp,ofa,ofi is used.

       -PSM2  Use this option to select the Intel Omni-Path Fabric. The application fails if no such fabric is found.

       -ofi   Use  this  option  to  select  an OpenFabrics Interfaces* (OFI*) capable network fabric. The application attempts to use an OFI-capable network fabric. If no such fabric is available,
              another fabric from the list tmi,dapl,tcp,ofa is used.

       -OFI   Use this option to select an OFI-capable network fabric. The application fails if no such fabric is found.


ENVIRONMENT VARIABLES FOR MPIEXEC.HYDRA
       I_MPI_HYDRA_HOST_FILE
              Set this environment variable to specify the hosts file.

       I_MPI_HYDRA_DEBUG
              Set this environment variable to enable the debug mode.

       I_MPI_HYDRA_ENV
              Set this environment variable to control the environment propagation to the MPI processes. By default, the entire launching node environment is passed to the  MPI  processes.  Setting
              this variable also overwrites environment variables set by the remote shell.

       I_MPI_JOB_TIMEOUT
              Set this environment variable to make mpiexec.hydra terminate the job in timeout seconds after its launch.

       I_MPI_JOB_TIMEOUT_SIGNAL
              Define  a signal number to be sent to stop the MPI job if the timeout period specified by the I_MPI_JOB_TIMEOUT environment variable expires. If you set a signal number unsupported by
              the system, the mpiexec.hydra command prints a warning message and continues the task termination using the default signal number 9 (SIGKILL).

       I_MPI_JOB_ABORT_SIGNAL
              Set this environment variable to define a signal for task termination. If you set an unsupported signal number, mpiexec.hydra prints a warning message and uses the  default  signal  9
              (SIGKILL)

       I_MPI_JOB_SIGNAL_PROPAGATION
              Set  this  environment variable to control propagation of the signals (SIGINT, SIGALRM, and SIGTERM). If you enable signal propagation, the received signal is sent to all processes of
              the MPI job. If you disable signal propagation, all processes of the MPI job are stopped with the default signal 9 (SIGKILL)

       I_MPI_HYDRA_BOOTSTRAP
              Set the I_MPI_HYDRA_BOOTSTRAP environment variable in the shell environment before executing the mpiexec.hydra command.

       I_MPI_HYDRA_BOOTSTRAP_EXEC
              Set this environment variable to specify the executable file to be used as a bootstrap server.

       I_MPI_HYDRA_BOOTSTRAP_EXEC_EXTRA_ARGS
              Set this environment variable to specify additional arguments for the bootstrap server.

       I_MPI_HYDRA_BOOTSTRAP_AUTOFORK
              Set this environment variable to control usage of fork call for the local processes.

       I_MPI_HYDRA_RMK
              Set this environment variable to use the pbs resource management kernel. Intel MPI Library only supports pbs.

       I_MPI_HYDRA_PMI_CONNECT
              Use this environment variable to select the PMI messages processing method.

       I_MPI_PMI2
              Set this environment variable to control the use of PMI-2 protocol.

       I_MPI_PERHOST
              Set this environment variable to define the default setting for the -perhost option. The -perhost option implied with the respective value if the I_MPI_PERHOST environment variable is
              defined.

       I_MPI_JOB_TRACE_LIBS
              Set this environment variable to choose an alternative library for preloading through the -trace option.

       I_MPI_JOB_CHECK_LIBS
              Set this environment variable to choose an alternative library for preloading through the -check_mpi option.

       I_MPI_HYDRA_BRANCH_COUNT
              Set this environment variable to restrict the number of child management processes launched by the mpiexec.hydra operation or by each pmi_proxy management process.

       I_MPI_HYDRA_PMI_AGGREGATE
              Set this environment variable to enable/disable aggregation of PMI messages.

       I_MPI_HYDRA_GDB_REMOTE_SHELL
              Set  this  environment variable to specify the remote shell command to run the GNU* debugger on the remote machines. You can use this environment variable to specify any shell command
              that has the same syntax as SSH or RSH.

       I_MPI_HYDRA_IFACE
              Set this environment variable to specify the network interface to use.

       I_MPI_HYDRA_DEMUX
              Set this environment variable to specify the multiple I/O demux mode engine. The default value is poll

       I_MPI_HYDRA_CLEANUP
              Set the I_MPI_HYDRA_CLEANUP environment variable to create the input file for the mpicleanup utility.

       I_MPI_TMPDIR
              Set this environment variable to specify the temporary directory to store the mpicleanup input file.

       I_MPI_JOB_RESPECT_PROCESS_PLACEMENT
              Specify whether to use the job scheduler provided process-per-node parameter.

       I_MPI_GTOOL
              Specify the tools to be launched for selected ranks. An alternative to this variable is the -gtool option.

       I_MPI_HYDRA_USE_APP_TOPOLOGY
              Set this variable to optimally place MPI ranks on the cluster.

       I_MPI_HYDRA_TOPOLIB
              Set this environment variable to define the interface for platform detection.

       I_MPI_HYDRA_PREFORK
              Set this environment variable to control the usage of the new processes startup method.


RETURN STATUS
       mpiexec returns the maximum of the exit status values of all of the processes created by mpiexec .



COPYRIGHT INFORMATION
       Intel, the Intel logo are trademarks of Intel Corporation in the U.S. and other countries.

       * Other names and brands may be claimed as the property of others.

       (C) Intel Corporation.



                                                                                                 2016                                                                                mpiexec.hydra(1)

# Intro_ParcoH1-2024-2025

## Table of Contents
- [Introduction](#introduction)
- [Software and Cluster Requirements](#software-and-cluster-requirements)
- [Installation and Configuration](#installation-and-configuration)
  - [Global Protect](#global-protect)
  - [Clone Git](#clone-git)
  - [Cluster](#cluster)
    - [Access Cluster](#access-cluster)
    - [Import Project](#import-project)
    - [Import Libraries](#import-libraries)
    - [Run Project](#import-libraries)
    - [Download Results](#download-results)
- [Project Layout](#project-layout)
- [Code Overview](#code-overview)
  - [Main](#main)
  - [Functions](#functions)
- [Contact](#contact)
  

# Introduction

The purpose of this project is studying various ways to optimize a matrix transposition starting from a sequential code. All the analysis and reflection on the result obtained can be found in the report, but to get a more in-depth of all the simulations done to do this project and the definitive results obtained you can see the folder results. The ways that I explored the Transposition and Symmetry are:<br><br>
(1) **Sequential Code**: code running normaly without any optimization<br>
(2) **Implicit Block**: implicit code (only one thread) using a block-based technique<br>
(3) **Implicit Recursive**: implicit code using a recursive function<br><br>
**OMP**:<br><br>
Symmetry check was done in OMP with two algorithms, one exiting when the condition is respected no more (called Local) and one that does all the iterations without exiting (called global).<br>
Transposition was done in a work-sharing and in a block-based technique.<br>
The combionation of these 4 generated these explored ways:<br><br>
(4) **Local Work-Sharing**<br>
(5) **Global Work-Sharing**<br>
(6) **Local Block-Based**<br>
(7) **Global Block-Based**<br><br>
This is a project done by Matteo Gottardelli which is the owner and responsible of any element in this repository.<br>

# Software and Cluster Requirements
- **Cluster**:<br>
All the simulations you can find in the results folder have been done on a cluster offered by the University of Trento. These were all done in a node that had a minimum of 64 cpus for standard purposes and for OpenMP and for safe I reserved 1 Gb of memory, which is x4 the memory needed for the maximum size.<br>
There are the following properties of the node on which I ran the simulations:<br>
Architecture:          x86_64<br>
CPU op-mode(s):        32-bit, 64-bit<br>
Byte Order:            Little Endian<br>
CPU(s):                96<br>
On-line CPU(s) list:   0-95<br>
Thread(s) per core:    1<br>
Core(s) per socket:    24<br>
Socket(s):             4<br>
NUMA node(s):          4<br>
Vendor ID:             GenuineIntel<br>
CPU family:            6<br>
Model:                 85<br>
Model name:            Intel(R) Xeon(R) Gold 6252N CPU @ 2.30GHz<br>
Stepping:              7<br>
CPU MHz:               2300.000<br>
BogoMIPS:              4600.00<br>
Virtualization:        VT-x<br>
L1d cache:             32K<br>
L1i cache:             32K<br>
L2 cache:              1024K<br>
L3 cache:              36608K<br>
NUMA node0 CPU(s):     0-23<br>
NUMA node1 CPU(s):     24-47<br>
NUMA node2 CPU(s):     48-71<br>
NUMA node3 CPU(s):     72-95<br><br>
- **Native Machine**:<br>
This project was developed with a MacOS machine, the simulation wasn't done on this machine, but I've said this to evidence that I didn't work with MobaxTerm (an application that gives a GUI interface to the user interfacing with a cluster). The download page for this application is right here:<li>https://mobaxterm.mobatek.net/download.html<li>


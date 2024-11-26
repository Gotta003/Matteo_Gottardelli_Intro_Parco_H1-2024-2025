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
```bash
Architecture:          x86_64
CPU op-mode(s):        32-bit, 64-bit
Byte Order:            Little Endian
CPU(s):                96
On-line CPU(s) list:   0-95
Thread(s) per core:    1
Core(s) per socket:    24
Socket(s):             4
NUMA node(s):          4
Vendor ID:             GenuineIntel
CPU family:            6
Model:                 85
Model name:            Intel(R) Xeon(R) Gold 6252N CPU @ 2.30GHz
Stepping:              7
CPU MHz:               2300.000
BogoMIPS:              4600.00
Virtualization:        VT-x
L1d cache:             32K
L1i cache:             32K
L2 cache:              1024K
L3 cache:              36608K
NUMA node0 CPU(s):     0-23
NUMA node1 CPU(s):     24-47
NUMA node2 CPU(s):     48-71
NUMA node3 CPU(s):     72-95
```
You can see these information on a Linux system using:
```bash
lscpu
```
- **Native Machine**:<br>
This project was developed with a MacOS machine, the simulation wasn't done on the cluster, but I've said this to evidence that I didn't work with MobaxTerm (an application that gives a GUI interface to the user interfacing with a cluster). The download page for this application is right here an if you have a windows architecture it's highly recommended to use this: [mobaxterm](https://mobaxterm.mobatek.net/download.html)<br><br>
Some operations with Moba aren't necessary to use the instructions in this readMe, like transfering files from and to the cluster.<br>

# Installation and Configuration

## Global Protect
If you would like to connect to a cluster, the connection has to be protected by a VPN coonection, so to install and setup a secure connection to setup the cluster:<br>
1. Download Global Protect according to version of your architecture: [Global Protect](https://vpn-esterni.icts.unitn.it/global-protect/getsoftwarepage.esp)<br>
2. Open Global Protect App<br>
3. Connection to a vpn portal. If you are part of the Unitn University, you can set the Portal to:
```bash
   vpn-mfa.icts.unitn.it
```
4. Pressing connect you will be redirect to an external login page and putting the credentials the connection will be set
## Clone Git
1. Access to your GitHub account, running the command below in the terminal on your computer:
```bash
cd ~
gh auth login
```
If it doesn't work you may have to download some extra dependencies, like gh.
If you are on MacOS or Linux you can install homebrew. To verify if homebrew is already install run on terminal:
```bash
brew --version
```
If that doesn't put out Homebrew and the version, it does mean you haven't installed it, yet. So, to install it run:
```bash
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
```
Now, doing:
```bash
brew install <missing command>
```
brew will be updated and that command will be integrated in your terminal. So, install with this method gh<br><br>

2. After the command run successfully, you will have to do some choices. Select Github.com then HTTPS, type Y and press Login with a web browser and then press enter. You can use any other type of method, but I'm going to explain it how I have done it<br>
3. In the new open panel, insert the credentials and the one-time code generated on terminal and authorize the connection
4. Insert your passkey and after that you will see on your terminal as last line "Logged in as <Your ID on GitHub><br>
5. Now run this command to clone the above repository:
```bash
gh repo clone Gotta003/Matteo_Gottardelli_Intro_Parco_H1-2024-2025
```
6. To see, if you have successfully clone the repository try to access to it, running:
```bash
cd Matteo_Gottardelli_Intro_Parco_H1-2024-2025
```
Now, your are in the folder.
3. 
## Cluster 
### Access Cluster
1. Now leaving the previous terminal window of the cloned repository, but it has to still be open, open a new terminal window
2. Run the command to access to the cluster (be careful that the VPN is active:
```bash
ssh <username>@hpc.unitn.it
```
If you have another cluster change the obvious parameters.
3. Insert the required password
4. In the University of Trento, I you are inside the cluster you will see your username@<one of two node in the cluster (n1 and n2)>
### Import Project
All the required codes that have to be run are in the "Matrix Transposition" folder, so we have to transfer that folder on the cluster. Now you should have two terminal windows, one in the cloned repository and another connected to the cluster. If so, follow these steps:
1. Run this command in the cloned repository terminal:
```bash
cd Matrix_Transposition
pwd
```
If not present or gives error, install with (if on MacOS or Linux):
```bash
brew install pwd
```
2. Make an alias, copy this instruction, but substitute with <Path From pwd> the path outputed previously
```bash
local_path='<Path From pwd>'
```
If that doesn't work, maybe the '' are indexed different, so change them manually
3. Create another alias with your email of the cluster you have logined with, if you are part of Unitn Cluster it is like:
```bash
cluster_mail='<username>@hpc.unitn.it'
```
4. Knowing that you would like to load all files in a specific directory, save in an alias the destination path of this folder, with you what it to be in the position you are on the cluster terminal, you can run this (ON CLUSTER):
```bash
pwd
```
5. Copy this path and create on the other terminal running this:
```bash
cluster_path='<Path Destination>'
```
6. Now, if you have correctly set the alias, you can directly run this command, in the native terminal (NOT THE CLUSTER ONE):
```bash
scp -r $local_path $cluster_mail:$cluster_path
```
8. 
### Import Libraries

### Run Project

### Download Results

# Project Layout

# Code Overview

## Main

## Functions

# Contact

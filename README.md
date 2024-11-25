# Intro_ParcoH1-2024-2025

## Table of Contents
- [Introduction](#introduction)
- [Software and/or Cluster Requirements](#software-requirements)
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

# Software and/or Cluster Requirements



Code for ICFPC 2013
=========
Programs in this repository is a solver of ICFPC
(Internaltional Conference of Functional Programing, Programing Contest)
2013.  The event was held on Aug 9-11, 2013.
Please refer http://icfpc2013.cloudapp.net/ for the details.


Programs and Tools
=========

solver
---------
solver is the main program which suggests \\bv programs based on size, operations, input values, and output values.

Inputs
-----
> S
> N
> O_1
> O_2
> :
> O_N
> M
> I_1 E_1
> I_2 E_2
> :
> I_M E_M
> i_1 e_1
> i_2 i_2
> :

 * S : Size of the prepared program
 * N : The number of operators' types in the prepared program
 * O_1, O_2, ..., O_N : Operators used in the prepared program
 * M : The number of sets of I/Os
 * I_1, I_2, ..., I_M : Input values to validate suggested program
 * E_1, E_2, ..., E_M : Expected outputs of the program with the input values
 * i_1, e_1, i_2, e_2, ... : Counter example I/Os got in guessing programs.  The number of them depends on what program solver suggests.  There may be no counters.

Outputs
-----
> P_1
> P_2
> :

 * P_1, P_2 : Suggested programs.  The nubmer of them depends on the behavior of solver.

query.sh
--------
A wrapper of wget.

organizer.py
--------
Organizes whole trial of programs.
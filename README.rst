####################################################################################################
yop2p - Yet Other C++ Peer to Peer Library
####################################################################################################
****************************************************************************************************
oR YOosofan C++ Peer to Peer Library
****************************************************************************************************

Ideas
####################################################################################################

A true peer to peer library for every where even behind firewall and NAT using idea of full nodes with IP, full nodes without IP, routers, and trackers which can be incentivise them
for their help to secure the network based on how much they help the system.

Build
####################################################################################################

Linux
****************************************************************************************************

.. code:: sh


    mkdir -p build
    cd build
    cmake ../ 
    make
    cd ..

Run tests
====================================================================================================

For each tests in test/ directory an executable file is generated and can be run in build/ directory. For example

.. code:: sh

    cd build
    ./testp2p
    cd ..


cleaning directory
====================================================================================================

.. code:: sh

    bash clean.sh

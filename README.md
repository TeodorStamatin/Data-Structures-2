Copyright 2023 <Stamatin Teodor>

# Project 2 - Load Balancer
This program is a load balancer implemented in C language that uses hash tables
to distribute data among multiple servers. The program is composed of three
parts: the servers, the load balancer and the utils. The servers are the ones
that store the data, the load balancer is the one that distributes the data
among(us) the servers and the utils are the functions that are used by the servers
and the load balancer.

## Table of contents

- [Servers](#servers)
- [Utils](#utils)
- [LoadBalancer](#loadbalancer)
- [Conclusion](#conclusion)

## Servers
Each server contains a hashtable where the key-values pairs are stored, an id
and a replica number which makes the difference between the primary and the two
secondary servers. The functions are implemented in such a way that they do
what their description says. The only thing that is worth mentioning is that
HMAX is a macro used to compute the maximum number of buckets of the hashtable.

## Utils
The utils file contains the functions that are used by the servers and the load
balancer. The functions are mostly hashtable linked list functions (from the 
lab), with a bit of modification on the hashtable side.

## LoadBalancer
In order to create a new load balancer, you can use the init_load_balancer()
function. This function initializes an empty load balancer and returns a
pointer to it.
You can add a new server to the load balancer by using the loader_add_server
function. This function adds three replicas of the server, each with the same
server id but with a unique replica id, to the load balancer. The replica id
is used with the POWER macro to determine the server number, which is used to
determine the hash of the server. After the allocation of the three new server,
we sort the server by using sort_servers_by_hash function, and we rebalance the
data from all hashtables.
Removing a server from the load balancer can be done by using the
loader_remove_server function. This function removes all the replicas of the
specified server from the load balancer, and redistributes the data from the
removed servers to the remaining servers.
To store data in the load balancer, you can use the loader_store function. This
function searches for the closest server clock-wise based on the hash and
stores the key-value pair.
To retrieve data from the load balancer, you can use the server_retrieve
function. This function retrieves the value associated with the specified key
from the load balancer.
The free_load_balancer function frees the memory allocated for the load
balancer.

## Conclusion
This project was a bit more challenging than the first one, but it was a good
practice and learned a lot of new things. I had a lot of fun doing it and I
hope you will have fun reading it. All in all, I am pretty satisfied with the
result, although the code is very (un)efficient XD.
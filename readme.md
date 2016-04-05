
# starting enduro
$ cd /home/gritins/projects/endurox
$ . setndrx
$ xa start -y

# listing services
$ xa psc

# managing bridges
$ xa ppm
$ xa stop -i 600
$ xa start -i 102

# building and running the addon project
$ cd /home/gritins/projects/endurox
$ . setndrx
$ cd /home/gritins/osx
$ node-gyp rebuild
$ node server

# sample ATMI client
/home/gritins/projects/endurox/testclt/testclt.c
./testclt

# change and make
change testclt.c
make

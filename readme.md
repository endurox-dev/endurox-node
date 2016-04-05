
# starting enduro
$ cd /home/user1/projects/endurox

$ . setndrx

$ xa start -y

# listing services
$ xa psc

# building and running the addon project
$ cd /home/user1/projects/endurox

$ . setndrx

$ cd /home/user1/osx

$ node-gyp rebuild

$ node server

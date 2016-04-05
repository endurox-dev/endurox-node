
# Install Node.js

Download the Node.js Linux 64-bit binaries and extract the file, for example into /opt:

cd /opt
tar -Jxf node-v4.2.3-linux-x64.tar.xz

Set PATH to include Node.js:

export PATH=/opt/node-v4.2.3-linux-x64/bin:$PATH

# Install EnduroX
1. Download Linux package (DEB/RPM/tar.gz) from http://www.endurox.org/projects/endurox/files and install with your OS tools.
2. To configure full EnduorX enviroment follow http://www.endurox.org/dokuwiki/doku.php?id=v2.4.x:guides:getting_started_tutorial guide.

# Starting EnduroX (depends on your project) and how system was built
$ cd /home/user1/projects/endurox

$ . setndrx

$ xa start -y

# Listing services (these can be called from Node.js)
$ xa psc

# Building and running the addon project
1. $ git clone https://github.com/endurox-dev/endurox-node
2. $ cd endurox-node
3. $ node-gyp rebuild
4. $ node server

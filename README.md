md5check
======================
simple file integrity check tool.

install
======================
<pre>
  $ sudo apt-get install cmake

  $ git clone https://github.com/yoggy/md5check.git
    (or wget https://github.com/yoggy/md5check/archive/master.zip)

  $ cd md5check
  $ cmake .
  $ make
</pre>

usage
======================
<pre>
usage:
  md5watch is realtime new/modify/delete watching tool.

  usage:
      $ md5watch [file1] [file2] ... [dir1] [dir2] ...

  example:
      $ md5watch /var /tmp /usr/local
      2013/07/30 19:50:04 [D] start watching...
      2013/07/30 19:50:26 [D] watching...
      2013/07/30 19:50:57 [I] N,/var/.a
      2013/07/30 19:51:12 [I] M,/var/tmp/text.txt
      2013/07/30 19:53:35 [I] D,/tmp/qwerty
           .
           .
           .
      N : create new file
      M : modified file
      D : deleted file
</pre>

<pre>
  md5create & md5check is simple file integrity check tool.

  usage:
      $ md5create db_file [file1] [file2] ... [dir1] [dir2] ...
      $ md5check  db_file [file1] [file2] ... [dir1] [dir2] ...
      
  example:
      create md5 hash database.
        $ md5create db_file.db /var /tmp /usr/local

      check dm5 hash database.
        $ md5check db_file.db /var /tmp /usr/local
        N,/var/.a
        M./var/tmp/text.txt
        D,/tmp/qwerty
           .
           .
           .
</pre>


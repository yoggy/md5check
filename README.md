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

        N : create new file
        M : modified file
        D : deleted file
</pre>

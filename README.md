md5check
======================
now under construction...

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
</pre>

#poisson-anim.plt: Script for creating an animation from matrix data
#          author: Linas Kaminskas (s082936@student.dtu.dk)
#        modified: 23-10-2015
unset key
unset colorbox
unset xtics
set x2tics
set border linewidth 1.25
set palette grey
set lmargin -0.15;
set rmargin -0.15;
set bmargin -0.15;
set tmargin -0.15;
set size ratio 1
#set xrange [0:6]
#set yrange [0:6]
#set zrange [0:1]
set cbrange [0:1]
#set tics nomirror out scale 0.75
set grid
set yrange [:] reverse
set autoscale x2fix
set autoscale yfix
stats data_file nooutput
set terminal pngcairo enhanced crop
do for [i=0:(int(STATS_blocks)-1)] {
    set output sprintf("%s/%s-%016.0f.png",output_dir,frame_name,i)
    plot data_file index i u ($1+1):($2+1):3 matrix with image
}


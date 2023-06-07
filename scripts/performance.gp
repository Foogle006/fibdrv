set title "fibdrv performance"
set xlabel "n"
set ylabel "time"
set terminal png font " Times_New_Roman,12 "
set output "performance.png"
set key left 

plot \
"performance.txt" using 1:2 with linespoints linewidth 2 title "user", \
"performance.txt" using 1:3 with linespoints linewidth 2 title "kernel", \
"performance.txt" using 1:4 with linespoints linewidth 2 title "copy to user", \

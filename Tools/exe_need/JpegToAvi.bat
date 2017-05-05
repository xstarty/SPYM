mencoder mf://*.jpg -mf w=800:h=600:fps=1:type=jpg -ovc lavc -lavcopts vcodec=mpeg4:mbd=2:trell -oac copy -o %1.avi
del *.jpg
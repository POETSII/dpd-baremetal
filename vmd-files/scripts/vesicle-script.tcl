# Procedures for taking pictures
proc take_picture {args} {
  global take_picture

  # when called with no parameter, render the image
  if {$args == {}} {
    set f [format $take_picture(format) $take_picture(frame)]
    # take 1 out of every modulo images
    if { [expr $take_picture(frame) % $take_picture(modulo)] == 0 } {
      render $take_picture(method) $f
      # call any unix command, if specified
      if { $take_picture(exec) != {} } {
        set f [format $take_picture(exec) $f $f $f $f $f $f $f $f $f $f]
        eval "exec $f"
       }
    }
    # increase the count by one
    incr take_picture(frame)
    return
  }
  lassign $args arg1 arg2
  # reset the options to their initial stat
  # (remember to delete the files yourself
  if {$arg1 == "reset"} {
    set take_picture(frame)  0
    set take_picture(format) "./rgbs/animate.%04d.rgb"
    set take_picture(method) snapshot
    set take_picture(modulo) 1
    set take_picture(exec)    {}
    return
  }
  # set one of the parameters
  if [info exists take_picture($arg1)] {
    if { [llength $args] == 1} {
      return "$arg1 is $take_picture($arg1)"
    }
    set take_picture($arg1) $arg2
    return
  }
  # otherwise, there was an error
  error {take_picture: [ | reset | frame | format  | method  | modulo ]}
}
# to complete the initialization, this must be the first function
# called.  Do so automatically.

proc make_trajectory_movie_files {} {
    set num [molinfo top get numframes]
    molinfo top get numframes
    # loop through the frames
    for {set i 0} {$i < $num} {incr i} {
        # go to the given frame
        animate goto $i
                # force display update
                display update
        # take the picture
        take_picture
        }
}

set len 100

# Load first file so it generates a molecule
# Use waitfor -1 so it loads all frames before continuing
mol new 100-100-100-vesicle-1m-timesteps.pdb waitfor -1
# Add later files to the first molecule
# mol addfile ... waitfor -1

# Change representation
# Set drawing form to VDW at size 0.2
mol modstyle 0 0 VDW 0.2
# Set the colouring method to Element
mol modcolor 0 0 element
# Set the material to AOShiny
mol modmaterial 0 0 AOShiny

# Set the window size to be useful
display resize 672 848

# Set background colour to white
color Display Background white

# Set oil1 to orange
color Element H red
# Set oil2 to green
color Element X green

# Turn shadows and ambient occlusion on
display shadows on
display ambientocclusion on

# Rotate for a nice view
rotate y by 45

# Turn off axes
axes location off

# Zoom out a little
scale by 0.95

# Reset animation to 0
animate goto 800

# Draw a box so we can see where the edges are
pbc set {100, 100, 100} -all
pbc box

take_picture reset
make_trajectory_movie_files

exit

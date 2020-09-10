require_relative 'cv-sequencer/step-block'

margin = hp2mm(2.0)
width_hp = margin + STEP_BLOCK_WIDTH + margin

name "CV SEQUENCER X"

hp width_hp

hue = 30
foreground [hue, 100, 10]
background [hue, 10, 93]

left = margin
top = hp2mm(3.5)
bottom = hp2mm(23)


###############################################################################
#
# Step Controls
#
###############################################################################

step_block(top: top, bottom: bottom, left: left, foreground: @foreground, background: @background)

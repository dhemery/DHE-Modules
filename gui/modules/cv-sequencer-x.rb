require_relative 'cv-sequencer/style'
require_relative 'cv-sequencer/step-block'

name "CV SEQUENCER X"

WIDTH_HP = MARGIN_HP + STEP_BLOCK_WIDTH_HP + MARGIN_HP
hp WIDTH_HP

foreground FOREGROUND_HSL
background BACKGROUND_HSL

step_block(LEFT_MM)

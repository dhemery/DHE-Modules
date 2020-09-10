require_relative 'cv-sequencer/style'
require_relative 'cv-sequencer/step-block'

RIGHT_HP = MARGIN_HP + 9.0 + STEP_BLOCK_WIDTH_HP
WIDTH_HP = RIGHT_HP + MARGIN_HP

name "CV SEQUENCER"

hp WIDTH_HP

foreground FOREGROUND_HSL
background BACKGROUND_HSL

RIGHT_MM = hp2mm(RIGHT_HP)

###############################################################################
#
# Sequence Controls (Left)
#
###############################################################################

SEQUENCE_CONTROLS_TOP_MM = TOP_MM + hp2mm(2.78)
SEQUENCE_CONTROLS_BOTTOM_MM = BOTTOM_MM - Port::DIAMETER / 2.0 - 1.0
SEQUENCE_CONTROLS_DY_MM = (SEQUENCE_CONTROLS_BOTTOM_MM - SEQUENCE_CONTROLS_TOP_MM) / 4
RUN_Y_MM = SEQUENCE_CONTROLS_TOP_MM + 0 * SEQUENCE_CONTROLS_DY_MM
LOOP_Y_MM = SEQUENCE_CONTROLS_TOP_MM + 1 * SEQUENCE_CONTROLS_DY_MM
SELECTION_Y_MM = SEQUENCE_CONTROLS_TOP_MM + 2 * SEQUENCE_CONTROLS_DY_MM
GATE_Y_MM = SEQUENCE_CONTROLS_TOP_MM + 3 * SEQUENCE_CONTROLS_DY_MM
RESET_Y_MM = SEQUENCE_CONTROLS_TOP_MM + 4 * SEQUENCE_CONTROLS_DY_MM

SELECTION_LENGTH_DX_MM = hp2mm(1.63)
SELECTION_LENGTH_X_MM = LEFT_MM + SELECTION_LENGTH_DX_MM

input_button_port x: LEFT_MM, y: RUN_Y_MM, label: 'RUN'
input_button_port x: LEFT_MM, y: GATE_Y_MM, label: 'GATE'
connector left: LEFT_MM, right: SELECTION_LENGTH_X_MM, y: SELECTION_Y_MM
small_knob x: LEFT_MM - hp2mm(0.2), y: SELECTION_Y_MM, label: 'START'
small_knob x: SELECTION_LENGTH_X_MM, y: SELECTION_Y_MM, label: 'LENGTH'
input_button_port x: LEFT_MM, y: LOOP_Y_MM, label: 'LOOP'
input_button_port x: LEFT_MM, y: RESET_Y_MM, label: 'RESET'


###############################################################################
#
# Step Controls
#
###############################################################################

STEP_LEFT_MM = hp2mm(8.5)

LABEL_X_MM = STEP_LEFT_MM - 0.6 * STEP_DX_MM
label x: LABEL_X_MM, y: TRIGGER_Y_MM, text: 'TRIG', alignment: :left_of, size: :large
label x: LABEL_X_MM, y: INTERRUPT_Y_MM, text: 'INT', alignment: :left_of, size: :large
label x: LABEL_X_MM, y: COMPLETION_Y_MM, text: 'AT END', alignment: :left_of, size: :large
label x: LABEL_X_MM, y: START_ANCHOR_Y_MM, text: 'START', alignment: :left_of, size: :large
label x: LABEL_X_MM, y: END_ANCHOR_Y_MM, text: 'END', alignment: :left_of, size: :large
label x: LABEL_X_MM, y: SHAPE_Y_MM - hp2mm(0.25), text: 'SHAPE', alignment: :left_of, size: :large
label x: LABEL_X_MM, y: DURATION_Y_MM, text: 'DUR', alignment: :left_of, size: :large
label x: LABEL_X_MM, y: ENABLED_Y_MM, text: 'ON', alignment: :left_of, size: :large

step_block(LEFT_MM)




###############################################################################
#
# Sequence Controls (Right)
#
###############################################################################

NEAR_RIGHT_MM = RIGHT_MM - hp2mm(2.2)
OUT_Y_MM = BOTTOM_MM - Port::DIAMETER / 2.0 - 1.0
IN_Y_MM = SEQUENCE_CONTROLS_TOP_MM
POLARITY_Y_MM = (START_ANCHOR_Y_MM + END_ANCHOR_Y_MM) / 2.0
POLARITY_SPREAD_MM = (END_ANCHOR_Y_MM - START_ANCHOR_Y_MM) / 2.4

input_port x: NEAR_RIGHT_MM, y: IN_Y_MM, label: 'IN'
input_port x: RIGHT_MM, y: IN_Y_MM, label: 'AUX'
line x1: LEFT_MM + STEP_BLOCK_WIDTH_MM, x2: NEAR_RIGHT_MM, y1: POLARITY_Y_MM - POLARITY_SPREAD_MM, y2: POLARITY_Y_MM
line x1: LEFT_MM + STEP_BLOCK_WIDTH_MM, x2: NEAR_RIGHT_MM, y1: POLARITY_Y_MM + POLARITY_SPREAD_MM, y2: POLARITY_Y_MM
polarity_toggle x: NEAR_RIGHT_MM, y: POLARITY_Y_MM, selection: 2
connector left: LEFT_MM + STEP_BLOCK_WIDTH_MM, right: NEAR_RIGHT_MM, y: DURATION_Y_MM
duration_toggle x: NEAR_RIGHT_MM, y: DURATION_Y_MM
port x: RIGHT_MM, y: DURATION_Y_MM, label: 'CV'
output_port x: RIGHT_MM, y: OUT_Y_MM, label: 'OUT'

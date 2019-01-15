name 'BOOSTER STAGE'
hp 8
foreground [0, 100, 30]
background [0, 100, 97]

left = @width / 6.0 + 1.0 / 3.0
right = @width - left
center = @width / 2.0

y = 25.0
delta_y = 18.5

connector left: left, right: right, y: y
cv_port x: left, y: y
large_knob x: center, y: y, label: 'LEVEL'
polarity_toggle x: right, y: y

y += delta_y

connector left: left, right: right, y: y
cv_port x: left, y: y
large_knob x: center, y: y, label: 'CURVE'
shape_toggle x: right, y: y

y += delta_y

connector left: left, right: right, y: y
cv_port x: left, y: y
large_knob x: center, y: y, label: 'DURATION'
duration_toggle x: right, y: y

y = 82.0
delta_y = 15.0

input_button_port x: left, y: y, label: 'DEFER'
output_button_port x: right, y: y, label: 'ACTIVE'

y += delta_y

input_button_port x: left, y: y, label: 'TRIG'
output_button_port x: right, y: y, label: 'EOC'

y += delta_y

input_port x: left, y: y
output_port x: right, y: y


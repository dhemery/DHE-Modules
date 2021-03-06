name 'XYCLOID'
hp 11
foreground [270, 100, 50]
background [270, 66, 97]

left = width / 7.0
right = width - left
left_center = (right - left) / 3.0 + left
right_center = width - left_center

y = 25.0
delta_y = 18.5
port_offset = 1.25

connector(left: left, right: right_center, y: y)
cv_port(x: left, y: y)
attenuverter(x: left_center, y: y)
large_knob(x: right_center, y: y, label: 'SPEED')

y += delta_y

connector(left: left, right: right, y: y)
cv_port(x: left, y: y)
attenuverter(x: left_center, y: y)
large_knob(x: right_center, y: y, label: 'RATIO')
toggle(x: right, y: y, labels: %w(LOCK FREE), selection: 2)

y += delta_y

connector(left: left, right: right, y: y)
cv_port(x: left, y: y)
attenuverter(x: left_center, y: y)
large_knob(x: right_center, y: y, label: 'DEPTH')
toggle(x: right, y: y, labels: ['IN', ' ', 'OUT'], selection: 2)

y += delta_y

connector(left: left, right: right_center, y: y)
cv_port(x: left, y: y)
attenuverter(x: left_center, y: y)
large_knob(x: right_center, y: y, label: 'PHASE')

y = 97.0
delta_y = 15.0

connector(left: left, right: right, y: y)
cv_port(x: left, y: y)
small_knob(x: left_center, y: y, label: 'GAIN')
polarity_toggle(x: right_center, y: y)
output_port(x: right, y: y + port_offset, label: 'X OUT')

y += delta_y

connector(left: left, right: right, y: y)
cv_port(x: left, y: y)
small_knob(x: left_center, y: y, label: 'GAIN')
polarity_toggle(x: right_center, y: y)
output_port(x: right, y: y + port_offset, label: 'Y OUT')

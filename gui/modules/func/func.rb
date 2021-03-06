name 'FUNC'
hp 3
foreground [220, 100, 40]
background [40, 50, 96]

center = width / 2.0

top = 23.0
bottom = 108.0
row_count = 6
last_row = row_count - 1
delta_y = (bottom - top) / last_row

port_offset = 1.25

y = top
input_port(x: center, y: y + port_offset)

y += delta_y
toggle(x: center, y: y, labels: %w[ADD MULT], selection: 1)

y += delta_y
large_knob(x: center, y: y, label: ' ')

y += delta_y
stepper x: center, y: y, name: 'offset-range', options: %w[0–5 ±5 0–10 ±10], selection: 2, width: 8
stepper x: center, y: y, name: 'multiplier-range', options: %w[0–1 ±1 0–2 ±2], selection: 2, width: 8, hidden: true

y += 2.0 * delta_y
output_port(x: center, y: y + port_offset)

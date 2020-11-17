name 'BUTTONS'
hp 5
foreground [60, 100, 15]
background [60, 100, 97]

button_x = 5
negate_x = button_x + 6.5
port_x = negate_x + 7.7

top = 21
dy = 12.7

%w[A B C D E F G H].each_with_index do |name, i|
  button x: button_x, y: top + i * dy, label: name, label_size: :large
  button x: negate_x, y: top + i * dy, label: '¬', label_size: :large
  port x: port_x, y: top + i * dy
end

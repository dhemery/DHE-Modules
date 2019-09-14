name 'CURVE SEQUENCER 8'

steps = 8
width_hp = 31

curve_sequencer_file = Pathname('lib/modules/curve-sequencer.rb')
instance_eval(curve_sequencer_file.read, curve_sequencer_file.to_s)

name 'CURVE SEQUENCER 16'

steps = 16
width_hp = 49

curve_sequencer_file = Pathname('lib/modules/curve-sequencer.rb')
instance_eval(curve_sequencer_file.read, curve_sequencer_file.to_s)

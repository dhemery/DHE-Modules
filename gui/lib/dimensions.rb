MM_PER_HP = 5.08
MM_PER_INCH = 25.4
PX_PER_INCH = 75.0
PX_PER_MM = PX_PER_INCH / MM_PER_INCH

MODULE_HEIGHT = 128.5

STROKE_WIDTH = 0.35
PADDING = 1.0

def hp2mm(hp)
  hp * MM_PER_HP
end

def mm2hp(mm)
  mm / MM_PER_HP
end

def i2mm(mm)
  mm * MM_PER_INCH
end

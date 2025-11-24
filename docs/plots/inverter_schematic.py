import schemdraw
import schemdraw.elements as elm

with schemdraw.Drawing() as d:
    d.config(unit=2.0)

    # DC bus
    vplus = d.add(elm.SourceV().up().label('$V_{dc}$', 'left'))
    d.add(elm.Line().right(6))
    vminus = d.add(elm.Ground().at(vplus.start).label('0', 'left'))

    # Left leg (phase A)
    d.push()
    d.add(elm.Line().right(1))
    d.add(elm.Switch().down().label('$S_{a+}$', 'right'))
    n_a = d.add(elm.Dot(open=True))
    d.add(elm.Switch().down().label('$S_{a-}$', 'right'))
    d.add(elm.Line().down().to(vminus.start))
    d.pop()

    # Right leg (phase B)
    d.push()
    d.add(elm.Line().right(5))
    d.add(elm.Switch().down().label('$S_{b+}$', 'right'))
    n_b = d.add(elm.Dot(open=True))
    d.add(elm.Switch().down().label('$S_{b-}$', 'right'))
    d.add(elm.Line().down().to(vminus.start))
    d.pop()

    # Load between phase A and B
    d += elm.Resistor().at(n_a.center).to(n_b.center).label('Load', loc='top')

    d.draw()
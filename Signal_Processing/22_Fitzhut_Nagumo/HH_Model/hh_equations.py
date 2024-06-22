from numpy import exp

def a_n(v): 
    ''' alpha for variable n '''
    return (0.01*(v+55))/(1-exp(-(v+55)/10))

def a_m(v): 
    ''' alpha for variable m '''
    return (0.1*(v+40))/(1-exp(-(v+40)/10))

def a_h(v): 
    ''' alpha for variable h '''
    return 0.07*exp(-(v+65)/20)

def b_n(v): 
    ''' beta for variable n '''
    return 0.125*exp(-(v+65)/80)

def b_m(v): 
    ''' beta for variable m '''
    return 4*exp(-(v+65)/18)

def b_h(v): 
    ''' beta for variable h '''
    return 1/(1+exp(-(v+35)/10))

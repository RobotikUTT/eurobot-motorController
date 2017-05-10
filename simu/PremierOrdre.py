from cmath import exp

class PremierOrdre:
    def __init__(self, dt, K, Tau):
        self.dt  = dt
        self.K   = K
        self.Tau = Tau
        self.A   = exp(-self.dt/self.Tau).real
        self.reset()

    def reset(self):
        self.y   = 0
        
    def process(self, U):
        self.y = self.K*U + (self.y - self.K*U) * self.A

        return self.y

class FF:
    def __init__(self, dt, K, Tau):
        self.dt  = dt
        self.K   = K
        self.Tau = Tau
        self.A   = exp(-self.dt/self.Tau).real
        self.reset()

    def reset(self):
        self.y1   = 0
        
    def process(self, sp):
        cmd = (1/self.K) * (sp-self.y1*self.A) / (1 - self.A)

        return cmd

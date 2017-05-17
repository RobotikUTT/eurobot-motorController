class DigitalPID:
    def __init__(self, dt, kp, ki, kd):
        self.q0   = kp*(ki*dt/2+kd/dt+1)
        self.q1   = kp*(ki*dt/2-kd/dt*2-1)
        self.q2   = kp*kd/dt
        self.em1 = 0
        self.em2 = 0
        self.y   = 0

    def process(self, e):
        self.y  += self.q0 * e + self.q1 * self.em1 + self.q2 * self.em2
        self.em2 = self.em1
        self.em1 = e;

        return self.y

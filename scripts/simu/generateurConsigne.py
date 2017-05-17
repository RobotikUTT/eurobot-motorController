from sympy import *

def genTimePeriods(n, XpeakVals, T):
    X     = IndexedBase('X')
    Xpeak = IndexedBase('Xpeak')
    i, j  = symbols('i, j')

    TVals = [0 for k in range(n+1)]

    for p in range(n, 1-1, -1):
        A = (X[n]/2**(n)) * Product( Sum(2**j * T[n+1-i+j], (j, 0, i-1)).doit() + T[n+1-i], (i, 1, n)).doit() - X[0]

        subsDict = {}
        subsDict[X[0]] = XpeakVals[0]
        subsDict[X[n]] = XpeakVals[n]

        for k in range(1, n+1):
            if (k != p):
                subsDict[T[k]] = TVals[k]

        A = A.subs(subsDict)
        A = solve(A, T[p])
        print A
        
        for solution in A:
            if (solution.is_complex and round(im(solution), 3) != 0.00):
                continue

            if (solution.is_positive or round(solution) == 0):
                TVals[p] = round(solution, 3)

        for q in range(1, p-1+1):
            B = (X[n]/2**(n-q)) * Product( Sum(2**j * T[n+1-i+j], (j, 0, i-1)).doit() + T[n+1-i], (i, 1, n-q)).doit()

            subsDict = {}
            subsDict[X[n]] = XpeakVals[n]

            for k in range(1, n+1):
                subsDict[T[k]] = TVals[k]

            B = B.subs(subsDict)

            if (B > XpeakVals[q]):
                C = (X[n]/2**(n-q)) * Product( Sum(2**j * T[n+1-i+j], (j, 0, i-1)).doit() + T[n+1-i], (i, 1, n-q)).doit() - Xpeak[q]

                subsDict = {}
                subsDict[X[n]] = XpeakVals[n]
                subsDict[Xpeak[q]] = XpeakVals[q]

                for k in range(1, n+1):
                    if (k != p):
                        subsDict[T[k]] = TVals[k]

                C = C.subs(subsDict)
                C = solve(C, T[p])

                for solution in C:
                    if (solution.is_complex and round(im(solution), 3) != 0.00):
                        continue

                    if (solution.is_positive or round(solution) == 0):
                        TVals[p] = round(solution, 3)
            else:
                XpeakVals[q] = B

    return TVals, XpeakVals

def genSigns(n):
    if (n == 1):
        return [1]
    else:
        return genSigns(n-1) + [1] + [-i for i in genSigns(n-1)]


def genOrders(n, val):
    if (val == 1):
        return [n]
    else:
        previous = genOrders(n, val-1)
        new =  previous + [ n-val+1 ] + previous

        return new

def genPosCmd(n, XpeakVals):
    Xpeak = IndexedBase('Xpeak')
    T     = IndexedBase('T')
    i, j, t  = symbols('i, j, t', integer=True)

    signs       = genSigns(n)
    orders      = genOrders(n, n)
    timePeriods = [ T[order] for order in orders]
    timeFrames  = [T[0]] + [ sum(timePeriods[:i+1]) for i in range(len(timePeriods)) ]
    intervals = [[timeFrames[i], timeFrames[i+1]] for i in range(len(timeFrames)-1)]
    templates = [ [ 0 for inter in intervals] for i in range(n+1) ]

    for i in range(n, -1, -1):
        for j, order in enumerate(orders):
            peakValue = signs[j] * Xpeak[order]

            if (i == order):
                templates[i][j] = peakValue

            elif (i < n):
                func = integrate(templates[i+1][j], t)

                for k in range(j+1, len(orders)):
                    templates[i][k] += func.subs({ t: T[order] })

                templates[i][j] += func

    TVals, XpeakVals = genTimePeriods(n, XpeakVals, T)

    subsDict = { }
    for i in range(n+1):
        subsDict[T[i]] = TVals[i]
        subsDict[Xpeak[i]] = XpeakVals[i]

    def posCmd(t_):
        for i, template in enumerate(templates[0]):
            tA = intervals[i][0].subs(subsDict)
            tB = intervals[i][1].subs(subsDict)

            if (tA <= t_ and t_ <= tB):
                subsDict[t] = t_-tA
                return template.subs(subsDict)

        return XpeakVals[0]

    return posCmd, intervals[-1][1].subs(subsDict)

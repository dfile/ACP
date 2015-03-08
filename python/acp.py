# input: quad (a quadruple of curvatures)
# output: solutions (list of transformed quadruples)
# function: this runs quadruples through the four transformations (one for each curvature), and records the new one only if the transformed curvature is bigger than the original (so a smaller circle in the packing)
def transform(quad):
    solutions = []
    a, b, c, d = quad[0], quad[1], quad[2], quad[3]
    if (-a + (2 * (b + c + d))) > a:
        aPrime = [(-a + (2 * (b + c + d))), b, c, d]
        solutions.append(aPrime)
    if (-b + (2 * (a + c + d))) > b:
        bPrime = [a, (-b + (2 * (a + c + d))), c, d]
        solutions.append(bPrime)
    if (-c + (2 * (a + b + d))) > c:
        cPrime = [a, b, (-c + (2 * (a + b + d))), d]
        solutions.append(cPrime)
    if (-d + (2 * (a + b + c))) > d:
        dPrime = [a, b, c, (-d + (2 * (a + b + c)))]
        solutions.append(dPrime)
    return solutions

# input: quadList (a list of quadruples), ceiling (the arbitrary limit we don't want to go above)
# output: validQuads (list of quadruples where every curv is below the ceiling)
def check(quadList, ceiling):
    validQuads = []
    for quadruple in quadList:
        valid = True
        for entry in quadruple:
            if entry >= ceiling:
                valid = False
        if valid:
            validQuads.append(quadruple)
    return validQuads

# input: root (the initial quadruple that defines the packing), limit (arbitrary limit on curvatures we don't want to go above)
# output: ancestors (list of quadruples all below the limit)
def fuchsian(root, limit):
    ancestors = []
    ancestors.append(root)
    for parent in ancestors:
        nextGen = transform(parent)
        validGen = check(nextGen, limit)
        ancestors.append(validGen)
    return ancestors

# input: quadList (list of quadruples)
# output: actual (list of all the values of the input list)
# creates a list of unique curvatures
def valuesOf(quadList):
    possible, actual = [], []
    for ruple in quadList:
        for value in ruple:
            possible.append(value)
    maybe = set(possible)
    for element in maybe:
        actual.append(element)
    return actual

# input: quad (a quadruple), orbit (list of admissible quadruples)
# output: solutions (latest generation of admissible quadruples), orbit (list of updated admissible quadruples)
# define orbit: An orbit of a circle packing is all quadruples mod 24 which any quadruple in the packing will be equivalent to, no matter how far out you go. We're interested in finding orbits of arbitrary packings so we can check curvatures in them against it
def transformOrbit(quad, orbit):
    solutions = []
    family = [-1, -1, -1 ,-1]
    a, b, c, d = quad[0], quad[1], quad[2], quad[3]
    
    # the four matrices
    family[0] = [(-a + (2 * (b + c + d))) % 24, b % 24, c % 24, d % 24]
    family[1] = [a % 24, (-b + (2 * (a + c + d))) % 24, c % 24, d % 24]
    family[2] = [a % 24, b % 24, (-c + (2 * (a + b + d))) % 24, d % 24]
    family[3] = [a % 24, b % 24, c % 24, (-d + (2 * (a + b + c))) % 24]
    # print ("original orbit: ", orbit)
    for kid in family:
        if kid in orbit:
            pass
        else:
            orbit.append(kid)
            solutions.append(kid)
    return solutions, orbit

# input: seed (vector which represents the quadruple which defines the packing)
# output: orbit (list of all admissible quadruples [see above])
# genealogy is the host function which finds the complete orbit
def genealogy(seed):
    ancestors, orbit, modSeed = [], [], [0, 0, 0, 0]
    for i in range(4):
        # print("seed[i] = " , seed[i])
        modSeed[i] = (seed[i] % 24)
        # print("modSeed[i] = " , modSeed[i]);
    ancestors.append(modSeed)
    orbit.append(modSeed)
    for parent in ancestors:
        newGeneration, orbit = transformOrbit(parent, orbit)
        # print("orbit: ", orbit)
        # print("newGeneration: ", newGeneration)
        for offspring in newGeneration:
            ancestors.append(offspring)
    return orbit

# input: valList (admissible values of the packing), top (arbitrary limit)
# output: could (list of admissible values up to our arbitrary limit)
def path(valList, top):
    could = []
    i = 0
    while i < top:
        for poss in valList:
            if (i % 24) == poss:
                could.append(i)
                break
        i += 1
    return could

# input: root (quadruple which defines the packing), cap (arbitrary limit)
# output: gone (list of admissible values that DO NOT appear in the packing, though they should)
# host function for the last block of code, finds gone, the list we're looking for
def seek(root, cap):
    vecRoot = vector(ZZ, root)
    small = fuchsian(root, cap)
    valuesPack = valuesOf(small)
    admissible = genealogy(root)
    print("genealogy results:")
    print(admissible)
    valuesOrbit = valuesOf(admissible)
    valuesGlobal = path(valuesOrbit, cap)
    nope = set( valuesGlobal ).difference( valuesPack )
    missing = sorted( list(nope) )
    return missing
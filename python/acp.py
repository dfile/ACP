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

# input: root (the initial quadruple that defines the packing), limit (arbitrary limit we don't want to go above)
# output: ancestors (list of quadruples all below the limit)
def fuchsian(root, limit):
    ancestors = []
    ancestors.append(root)
    keepGoing = True
    while keepGoing:
        for parent in ancestors:
            nextGen = transform(parent)
            validGen = check(nextGen, limit)
            for child in validGen:
                ancestors.append(child)
        keepGoing = False
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
    a, b, c, d = quad[0], quad[1], quad[2], quad[3]
    
    # the four matrices
    aPrime = [(-a + (2 * (b + c + d))) % 24, b % 24, c % 24, d % 24]
    bPrime = [a % 24, (-b + (2 * (a + c + d))) % 24, c % 24, d % 24]
    cPrime = [a % 24, b % 24, (-c + (2 * (a + b + d))) % 24, d % 24]
    dPrime = [a % 24, b % 24, c % 24, (-d + (2 * (a + b + c))) % 24]
    # print("aPrime = ", aPrime, " bPrime = ", bPrime, " cPrime = ", cPrime, " dPrime = ", dPrime);
    family = [aPrime, bPrime, cPrime, dPrime]
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
    keepGoing = True
    while keepGoing:
        for parent in ancestors:
            newGeneration, orbit = transformOrbit(parent, orbit)
            # print("orbit: ", orbit)
            # print("newGeneration: ", newGeneration)
            for offspring in newGeneration:
                ancestors.append(offspring)
        keepGoing = False
    return orbit

# input: valsPack (list of values in the packing, obtained from fuchsian()), valsOrb (list of values in the orbit of a packing, obtained from genealogy()), limit (arbitrary limit we don't want to go above)
# output: missing (list of admissible curvatures which do NOT appear in the packing even though they should!!!!!!SHIFT+1!!!). tHIS IS THE REAL deal here. This is what all of this is meant to find. This is it. Srsly.
def compare(valsPack, valsOrb, limit):
    missing = []
    should = path(valsOrb, limit)
    # The reason compare is so slow is that the valsPack is always
    # searched in its entirety in every iteration of the for loop => O(n^2).
    # We should use a more efficient data structure such as a hashed set or a
    # binary search tree => ~O(1) or O(log(n)), respectively.
    for might in should:
        if might in valsPack:
            pass
        else:
            missing.append(might)
    return missing

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
    gone = compare(valuesPack, valuesOrbit, cap)
    return gone
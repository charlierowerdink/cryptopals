# ex1 string = "49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d"
# ex1 expectString = "SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29ub3VzIG11c2hyb29t"

# ex 2 hexStr1 = '1c0111001f010100061a024b53535009181c'
# ex 2 hexStr2 = '686974207468652062756c6c277320657965'
# ex 2 expXorString = '746865206b696420646f6e277420706c6179'

# ex 3 hexEncodeStr = 0x1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736


import io

def bitLen(int_type):
    length = 0
    while (int_type):
        int_type >>= 1
        length += 1
    return(length)

def convertHextoBase64(string):
	map64 = ['A', 'B','C', 'D', 'E', 'F','G', 'H', 'I', 'J','K', 'L', 'M', 'N','O', 'P', 'Q', 'R','S', 'T', 'U', 'V','W', 'X', 'Y', 'Z','a', 'b', 'c', 'd','e', 'f', 'g', 'h','i', 'j', 'k', 'l','m', 'n', 'o', 'p','q', 'r', 's', 't','u', 'v', 'w', 'x','y', 'z', '0', '1','2', '3', '4', '5','6', '7', '8', '9', '+', '/']
	convertedString = [] 
	
	#convert hex to binary
	binString = convertHexToBin(string)


	#convert binary to base64 using the map
	for i in range(0, len(binString), 6):
		decimal  = int(binString[i:i+6], 2)
		convertedString.append(map64[decimal])

	return ''.join(convertedString)


def convertHexToBin(string):
	binString = bin(int(string, 16))[2:]

	#pad with zeros
	while len(binString) < len(string) * 4:
			binString = '0' + binString

	return binString

def convertBinToHex(string):
	hexStr = hex(int(string, 2))[2:]

	return hexStr[:-1] # -1 for the 'L' python puts at the end of a number



def fixedXor(hexStr1, hexStr2):
	binHexStr1 = convertHexToBin(hexStr1)
	binHexStr2 = convertHexToBin(hexStr2)

	length1 = len(binHexStr1)
	length2 = len(binHexStr2)

	if length1 != length2:
		#print "ERROR: strings are not the same size"
		return 0

	xorStr = range(0, length1)
	for i in xorStr:
		xorStr[i] = str(int(binHexStr1[i] != binHexStr2[i]))

	return''.join(xorStr)


def decryptMessage(data):

	topChar = 0
	topStr = ''
	topCount = 0
	for char in range(0, 255):

		length = len(bin(data)) - 2
		charData = char
		for byte in range(0, length/8):
			charData = (charData << 8) | char
		#print hex(charData)

		# now just xor the charData with data
		message = data ^ charData

		formatMes = hex(message)[2:-1]
		if len(formatMes) % 2 == 1:
			formatMes = '0' + formatMes

		ascii = formatMes.decode("hex")
		
		asciiStr = str(ascii)
		
		count = 0
		count = asciiStr.count('a') + asciiStr.count('e') + asciiStr.count('i') + asciiStr.count('o') + asciiStr.count('t') + asciiStr.count(' ') 
		#count = count + asciiStr.count('A') + asciiStr.count('E') + asciiStr.count('I') + asciiStr.count('O') + asciiStr.count('T')

		if count > topCount:
			topChar = chr(char)
			topStr = asciiStr
			topCount = count

	return topStr, topCount


def decryptFile(pathToFile):
	f = open(pathToFile, 'r')

	topCount = 0
	topStr = ''
	for line in f:
		[string, count] = decryptMessage(int(line, 16))
		
		if count > topCount:
			topCount = count
			topStr = string

	f.close()

	print (topStr)
	print (topCount)


def encrypt(pathToFile):

	f = open(pathToFile, 'rb')
	fcrypt = open('5en.txt', 'wb')


	case = 'I'
	for line in f:
		for byte in line:
			fcrypt.write(bytearray(123))
			print ( case )
			if case == 'I':
				case = 'C'
			elif case == 'C':
				case = 'E'
			elif case == 'E':
				case = 'I'

				
        
	f.close()
	fcrypt.close()



  







if __name__ == '__main__':
	

	encrypt('5.txt')
	#print (encrypt(int(data2, 16)))
	
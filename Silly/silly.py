#/usr/bin/env python
"""
	Silly SunlineB[1][1] Code

 Author: Claire Blackshaw
 All rights reserved
"""

#Import Modules
import os, pygame, math, random
from pygame.locals import *

# Line by Line Intersection
def lineline(lineA, lineB):
	""" Line-line intersection algorithm, returns point of intersection or None """
	
	# ccw from http://www.lineA[1][1]ceboe.com/2006/10/23/line-segment-intersection-algorithm/
	def ccw(A,B,C):
		# print str(A) + ' # ' + str(B) + ' # ' + str(C)
		return (C[1]-A[1])*(B[0]-A[0]) > (B[1]-A[1])*(C[0]-A[0])
			
	if ccw(lineA[0],lineB[0],lineB[1]) != ccw(lineA[1],lineB[0],lineB[1]) and \
		 ccw(lineA[0],lineA[1],lineB[0]) != ccw(lineA[0],lineA[1],lineB[1]):
		 
			# formula from http://local.wasp.uwa.edu.au/~pbourke/geometry/lineline2d/
			ua =    float(((lineB[1][0]-lineB[0][0])*(lineA[0][1]-lineB[0][1]))-((lineB[1][1]-lineB[0][1])*(lineA[0][0]-lineB[0][0]))) / \
							float(((lineB[1][1]-lineB[0][1])*(lineA[1][0]-lineA[0][0]))-((lineB[1][0]-lineB[0][0])*(lineA[1][1]-lineA[0][1])))
			ub =    float(((lineA[1][0]-lineA[0][0])*(lineA[0][1]-lineB[0][1]))-((lineA[1][1]-lineA[0][1])*(lineA[0][0]-lineB[0][1]))) / \
							float(((lineB[1][1]-lineB[0][1])*(lineA[1][0]-lineA[0][0]))-((lineB[1][0]-lineB[0][0])*(lineA[1][1]-lineA[0][1])))
			return  ( lineA[0][0]+(ua*(lineA[1][0]-lineA[0][0])), \
							  lineA[0][1]+(ua*(lineA[1][1]-lineA[0][1])) )
	return None

# Message Handler Hacked Together
class GameMsg:
	def __init__(self, msgType):
		self.type = msgType

clock = pygame.time.Clock()
ListenerStack = []
def PostMessage(msg):
	for listner in ListenerStack:
		if listner.processMessage(msg) == 'StopMsg':
			return

class SillyTest:
	def __init__(self):
		self.msgLog = []
		self.textLog = []
		self.timer = 0
		self.edFont = pygame.font.Font(pygame.font.get_default_font(),16)
		self.setup()
		return
		
	def setup(self):
		# Add to Listeners
		ListenerStack.append(self)
		
		self.setupStage()
		
		# Send Startup Message
		msg = GameMsg('Start Game')
		PostMessage(msg)
		return
		
	def setupStage(self):
		# Setup Point List
		self.lineList = []
		self.openPointList = []
		
		# Add Screen Bounds
		self.lineList.append( ((150, 50), (650, 50)) )
		self.lineList.append(	((150,550), (650,550)) )
		self.lineList.append(	((150, 50), (150,550)) )
		self.lineList.append(	((650, 50), (650,550)) )
		
		# Add Some Random Points
		self.openPointList.append( (200,300) )
		self.openPointList.append( (600,300) )
		self.openPointList.append( (400,100) )
		self.openPointList.append( (400,500) )
		
		return
			
	def update(self): # FIXED TIME STEP
		self.timer += 1
		if self.timer > 5:
			self.timer = 0
			self.addLine(1)
		return
			
	def draw(self, screen):
		for line in self.lineList:
			pygame.draw.aaline(screen, (0, 0, 255), line[0], line[1])

		return
	
	def processEvent(self, event):
		# Key Down Events            
		if event.type == KEYDOWN:
			if event.key == K_ESCAPE:
				pygame.event.post(pygame.event.Event(pygame.QUIT))
				return
			elif event.key == K_SPACE:
				self.setupStage()
				for x in range(50):
					self.addLine(1)
			elif event.key == K_a:
				for x in range(50):
					self.addLine(1)
							
	def processMessage(self, msg):
		self.textLog.append(('MSG [', msg.type,']'))
		self.msgLog.append(msg)
		
		if msg.type == 'Start Game':
			print '=================== START GAME ==================='
			self.timer = 0
		else:
			print '___________Unkown Message [' + msg.type + ']_________'
			
	def checkPoint(self, dstPt):
		# Check for Nearby Points
		for otherPt in self.openPointList:
			if (math.pow(otherPt[0] - dstPt[0], 2) + math.pow(otherPt[1] - dstPt[1], 2)) < 100.0:
				return otherPt
		return None
		
	def checkLine(self, newLine):
		for line in self.lineList:
			if lineline(line, newLine) is not None:
				return False
		return True
				
	def addLine(self, count):
		if count > 10:
			return
		
		srcPt = random.choice(self.openPointList)
		rndDegree = random.random() * math.pi * 2.0
		rndLength = random.gauss(40,30)
		dstPt = (srcPt[0] + (math.sin(rndDegree) * rndLength), srcPt[1] + (math.cos(rndDegree) * rndLength))
		
		# Remove Overused Points
		if str(self.lineList).count(str(srcPt)) > 5:
			self.openPointList.remove(srcPt)
		
		# Check for Close point to avoid Dups
		closePt = self.checkPoint(dstPt)
		
		if closePt == None:
			# Check for Overlaps
			if self.checkLine((srcPt, dstPt)) == False:
				self.addLine(count + 1)
				return
					
			# Add Line
			self.openPointList.append( dstPt )
			self.lineList.append( (srcPt, dstPt) )
				
		else:
			# Check Line Doesn't already exsist AB or BA
			if str((srcPt, closePt)) in str(self.lineList) or str((closePt, srcPt)) in str(self.lineList):
				self.addLine(count + 1)
				return		
			
			# Check for Overlaps
			for line in self.lineList:
				if lineline(line, (srcPt, closePt)) is not None:
					self.addLine(count + 1)
					return
					
			# Add Line
			self.openPointList.append( closePt )
			self.lineList.append( (srcPt, closePt) )
			
		if (len(self.lineList) % 25) == 0:
			# Sanity Check
			self.SanityCheckOpenPoints()
			
	def SanityCheckOpenPoints(self):
		sanityLines = ( (0,+10), (0,-10), (+10,0), (-10,0) )
		removeCount = 0
		totalCount = len(self.openPointList)
		
		for point in self.openPointList:
			sanCount = 0
			for sanMod in sanityLines:
				sanLine = (point, ((point[0] + sanMod[0]), (point[1] + sanMod[1])) )
				if self.checkLine(sanLine) == True:
					sanCount += 1
			if sanCount < 2:
				self.openPointList.remove(point)
				removeCount += 1
		
		#print str(totalCount - removeCount) + '/' + str(totalCount)
		if removeCount > 0:
			print 'Removed Points:' + str(removeCount)

def main():
	"""this function is called when the program starts.
	it initializes everything it needs, then runs in
	a loop until the function returns."""
	
	#Initialize Everything
	pygame.init()
	pygame.font.init()
	screen = pygame.display.set_mode((800, 600))
	pygame.display.set_caption('Silly SunlineB[1][1] with Kimau')
	pygame.mouse.set_visible(0)

	#Create The Backgound-
	background = pygame.Surface(screen.get_size())
	background = background.convert()
	background.fill((10, 10, 10))

	#Display The Background
	screen.blit(background, (0, 0))
	pygame.display.flip()
	
	# Setup the Game State
	currState = SillyTest()

#Main Loop
	while 1:
		clock.tick(60)

		# Handle Input Events        
		for event in pygame.event.get():
			if event.type == QUIT:
				print 'Requested Exit'
				return
			elif event.type == pygame.USEREVENT:
				print 'Changing State'                
			else:
				currState.processEvent(event)
			
		# Update
		currState.update()

		# Draw Everything
		screen.blit(background, (0, 0))
		currState.draw(screen)
		pygame.display.flip()
					
	#Game Over
	print "Game Over"

#this calls the 'main' function when this script is executed
if __name__ == '__main__': main()
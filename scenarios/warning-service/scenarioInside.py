import storyboard
import timeline

def createStories(board):
    timeCondition = storyboard.TimeCondition(timeline.seconds(260))
    carSetCondition = storyboard.CarSetCondition("brokenVehicle")
    signalEffect = storyboard.SignalEffect("inside")
    condition = storyboard.AndCondition(timeCondition, carSetCondition)
    story = storyboard.Story(condition, [signalEffect])
    board.registerStory(story)

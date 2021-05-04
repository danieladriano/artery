import storyboard
import timeline

def createStories(board):
    timeCondition = storyboard.TimeCondition(timeline.seconds(17))
    carSetCondition = storyboard.CarSetCondition("brokenVehicle")
    signalEffect = storyboard.SignalEffect("fake_message")
    condition = storyboard.AndCondition(timeCondition, carSetCondition)
    story = storyboard.Story(condition, [signalEffect])
    board.registerStory(story)

import storyboard
import timeline

def createStories(board):
    timeCondition1 = storyboard.TimeCondition(timeline.seconds(5))
    carSetCondition1 = storyboard.CarSetCondition("brokenVehicle1")
    signalEffect1 = storyboard.SignalEffect("stop")
    condition1 = storyboard.AndCondition(timeCondition1, carSetCondition1)
    story1 = storyboard.Story(condition1, [signalEffect1])

    timeCondition2 = storyboard.TimeCondition(timeline.seconds(350))
    carSetCondition2 = storyboard.CarSetCondition("brokenVehicle1")
    signalEffect2 = storyboard.SignalEffect("scenario_1")
    condition2 = storyboard.AndCondition(timeCondition2, carSetCondition2)
    story2 = storyboard.Story(condition2, [signalEffect2])

    board.registerStory(story1)
    board.registerStory(story2)

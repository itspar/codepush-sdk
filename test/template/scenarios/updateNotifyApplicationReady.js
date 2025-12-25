var CodePushWrapper = require("../codePushWrapper.js");
import CodePush from "@d11/codepush";

module.exports = {
    startTest: function (testApp) {
        testApp.readyAfterUpdate();
        CodePush.notifyAppReady();
    },

    getScenarioName: function () {
        return "Good Update";
    }
};
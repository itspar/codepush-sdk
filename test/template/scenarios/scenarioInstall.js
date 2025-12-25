var CodePushWrapper = require("../codePushWrapper.js");
import CodePush from "@d11/codepush";

module.exports = {
    startTest: function (testApp) {
        CodePushWrapper.checkAndInstall(testApp, undefined, undefined, CodePush.InstallMode.IMMEDIATE);
    },

    getScenarioName: function () {
        return "Install";
    }
};
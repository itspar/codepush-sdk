var CodePushWrapper = require("../codePushWrapper.js");
import CodePush from "@d11/codepush";

module.exports = {
    startTest: function (testApp) {
        CodePushWrapper.sync(testApp, undefined, undefined, { installMode: CodePush.InstallMode.IMMEDIATE });
        CodePushWrapper.sync(testApp, undefined, undefined, { installMode: CodePush.InstallMode.IMMEDIATE });
    },

    getScenarioName: function () {
        return "Sync 2x";
    }
};
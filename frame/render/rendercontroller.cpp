#include "rendercontroller.h"

#include <tnotification.h>
#include <QSharedPointer>

struct RenderControllerPrivate {
    QList<RenderJobPtr> jobs;
};

RenderController::RenderController(QObject *parent) : QObject(parent)
{
    d = new RenderControllerPrivate();
}

RenderController* RenderController::instance()
{
    static RenderController* instance = new RenderController();
    return instance;
}

void RenderController::queueRenderJob(RenderJobPtr job)
{
    connect(job.data(), &RenderJob::stateChanged, this, [=](RenderJob::State state) {
        switch (state) {
            case RenderJob::Idle:
                break;
            case RenderJob::Started:
                break;
            case RenderJob::Finished: {
                tNotification* notification = new tNotification();
                notification->setSummary(tr("Render Job Complete"));
                notification->setText(tr("%1 has finished rendering.").arg(job->jobDisplayName()));
                notification->post(true);
                break;
            }
            case RenderJob::Errored: {
                tNotification* notification = new tNotification();
                notification->setSummary(tr("Render Job Failed"));
                notification->setText(tr("%1 failed to render.").arg(job->jobDisplayName()));
                notification->post(true);
                break;
            }

        }
    });
    d->jobs.append(job);
    this->attemptStartNextJob();
}

QList<RenderJobPtr> RenderController::jobs()
{
    return d->jobs;
}

void RenderController::attemptStartNextJob()
{
    for (RenderJobPtr job : d->jobs) {
        switch (job->state()) {
            case RenderJob::Idle:
                job->startRenderJob();
                return;
            case RenderJob::Started:
                //Run one job at a time
                return;
            case RenderJob::Finished:
            case RenderJob::Errored:
                break;

        }
    }
}
